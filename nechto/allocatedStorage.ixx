export module allocatedStorage;
import staticAllocator;

import <memory>;
import <vector>;
import <atomic>;
import <mutex>;
import <thread>;
import <utility>;
import <cassert>;
import <iostream>;


export namespace nechto
{
	using ushort = unsigned short;
	const int maxNumOfAllocators = 65536;

	template <class TCon>//сей суперКостыль появился после 3 часов борьбы с непонятными ошибками
	class storageTerminal;//этот класс нужен для работы отдельных потоков с общим хранилищем

	template <class TCon>
	class allocatedStorage
	{
		static_assert(maxNumOfAllocators <= 65536, "65536 is maximum");
		static std::unique_ptr<staticAllocator<TCon>> content[maxNumOfAllocators]; //массив контейнеров
		static ushort occupancy[maxNumOfAllocators]; //массив занятости
		static std::atomic<ushort> sflag; //номер занимаемого контейнера
		static std::atomic<ushort> freeSpace; //количество созданных контейнеров

		static std::mutex changeAllocatorBlock;

		

		static void toFreeAllocator(ushort number)//освобождение контейнера
		{
			changeAllocatorBlock.lock();
			occupancy[sflag.fetch_add(-1)] = number;
			++freeSpace;
			changeAllocatorBlock.unlock();
		}
		static ushort getFreeAllocator()//взятие контейнера
		{
			changeAllocatorBlock.lock();
			--freeSpace;
			ushort number;
			while (true)
			{
				number = sflag.fetch_add(1);
				if (!content[occupancy[number]])
				{
					changeAllocatorBlock.unlock();
					content[occupancy[number]] = std::make_unique<staticAllocator<TCon>>();
					return number;
				}
				if (content[occupancy[number]]->freeSpace() > 256)
				{
					changeAllocatorBlock.unlock();
					return number;
				}
			}

		}

	public:

		static staticAllocator<TCon>* getAllocator(const ushort number)
		{
			assert((number > 0) && (number < maxNumOfAllocators));
			assert(content[number]);
			return content[number].get();
		}

		static ushort getFreeSpace()
		{
			return freeSpace;
		}
		static void reset()
		{//полный сброс хранилища. Опасная операция!
			changeAllocatorBlock.lock();
			for (int i = 0; i < maxNumOfAllocators; i++)
				occupancy[i] = i;
			sflag = 1;
			freeSpace = maxNumOfAllocators - 1;
			changeAllocatorBlock.unlock();
		}

		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		friend class storageTerminal<TCon>;
		static thread_local storageTerminal<TCon> terminal;

	};

	//////////////////////////////////////////////////////////////////////////////////////////////
	//епический бредоМаразм https://www.cyberforum.ru/cpp-beginners/thread1143095.html
	template<class TCon>
	std::unique_ptr<staticAllocator<TCon>> allocatedStorage<TCon>::content[maxNumOfAllocators]; 
	//массив контейнеров
	template<class TCon>
	ushort allocatedStorage<TCon>::occupancy[maxNumOfAllocators]; //массив занятости
	template<class TCon>
	std::atomic<ushort> allocatedStorage<TCon>::sflag = 1; //номер занимаемого контейнера
	template<class TCon>
	std::atomic<ushort> allocatedStorage<TCon>::freeSpace
		= maxNumOfAllocators - 1; //количество созданных контейнеров
	template<class TCon>
	std::mutex allocatedStorage<TCon>::changeAllocatorBlock;
	

	template<class TCon>
	class storageTerminal
	{//реализовано отдельным классом для корректной работы конструктора и деструктора
		std::vector<ushort> localAllocatorSet;//список номеров зарезервированных контейнеров
		ushort currentAllocatorNumber;//номер текущего контейнера
		staticAllocator<TCon>* currentAllocator;//текущий контейнер

		//смена используемого в данный момент контейнера
		void changeCurrentAllocator()
		{
			for (auto i = localAllocatorSet.begin(); i != localAllocatorSet.end(); ++i)
			{
				if (allocatedStorage<TCon>::getAllocator(*i)->freeSpace() > 256)
				{
					currentAllocatorNumber = *i;
					
					currentAllocator = allocatedStorage<TCon>::getAllocator(*i);
					return;
				}
			}

			currentAllocatorNumber = allocatedStorage<TCon>::getFreeAllocator();
			std::cout << currentAllocatorNumber << std::endl;
			currentAllocator = allocatedStorage<TCon>::getAllocator(currentAllocatorNumber);
			localAllocatorSet.push_back(currentAllocatorNumber);
		}//сменить текущий контейнер
		static bool isFistTerminal;//первый терминал перезагружает хранилище
	public:
		
		explicit storageTerminal()
		{
			if (isFistTerminal)
			{
				allocatedStorage<TCon>::reset();
				isFistTerminal = false;
			}
			changeCurrentAllocator();			
		}
		~storageTerminal()
		{
			for (auto i = localAllocatorSet.begin(); i != localAllocatorSet.end(); ++i)
			{
				allocatedStorage<TCon>::toFreeAllocator(*i);//высвобождает используемые контейнеры, делая их доступными для других терминалов
			}
		}
		const std::pair<ushort, ushort> allocate()
		{
			assert(currentAllocator != nullptr);
			if (currentAllocator->freeSpace() <= static_cast<ushort>(256))
				changeCurrentAllocator();
			std::pair<ushort, ushort> id;
			id.first = currentAllocatorNumber;
			id.second = currentAllocator->allocate();
			return id;
		}
		void deallocate(const std::pair<ushort, ushort> id)
		{
			allocatedStorage<TCon>::getAllocator(id.first)->deallocate(id.second);
		}
	};
	
	template<class TCon>
	bool storageTerminal<TCon>::isFistTerminal = true;

	template<class TCon>
	thread_local storageTerminal<TCon> allocatedStorage<TCon>::terminal;
}
#pragma once
#include "typeDeclarations.h"
#include <atomic>
#include <mutex>
#include <vector>
#include <thread>

namespace nechto
{
	class nodePtr;
	class creator;
}
namespace nechto::nodeStorage
{
	class threadLocalAllocator;
	class nodeAllocator
	{
		nodeData content[65536]; //массив элементов
		ushort occupancy[65536]; //массив занятости

		//номер занимаемого элемента
		std::atomic<ushort> gflag = 0;

		//номер освобождаемого элемента
		std::atomic<ushort> sflag = 0;
		//свободное место
		std::atomic<ushort> freespace = 65535; 

		friend class nodePtr;
		friend class threadLocalAllocator;
	public:

		nodeAllocator()
		{//заполнение списка занятости
			for (int i = 0; i <= 65535; i++)
				occupancy[i] = i;
		}
		nodeData* get(const ushort number)
		{
			return &content[number];
		}
		nodeData* operator[](const ushort number)
		{
			return &content[number];
		}


		void deallocate(ushort number) //освобождение элемента
		{
			++freespace;
			occupancy[sflag.fetch_add(1)] = number;
		}
		ushort allocate() //взятие элемента
		{
			--freespace;
			return occupancy[gflag.fetch_add(1)];
		}
		ushort freeSpace() //количество элементов в очереди
		{
			return freespace.load();
		}
	};

	const int maxNumOfAllocators = 16; //максимальное количество выделяемых аллокаторов
	static std::unique_ptr<nodeAllocator> content[maxNumOfAllocators]; //массив контейнеров
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
				content[occupancy[number]] = std::make_unique<nodeAllocator>();
				return number;
			}
			if (content[occupancy[number]]->freeSpace() > 256)
			{
				changeAllocatorBlock.unlock();
				return number;
			}
		}

	}

	nodeAllocator* getAllocator(const ushort number)
	{
		assert((number > 0) && (number < maxNumOfAllocators));
		if (!content[number])
		{
			std::wcout << L"allocator number " << number << L" does not exist" << std::endl;
			assert(false);
		}
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

	class threadLocalAllocator
	{//реализовано отдельным классом для корректной работы конструктора и деструктора
		std::vector<ushort> localAllocatorSet;//список номеров зарезервированных контейнеров
		ushort currentAllocatorNumber;//номер текущего контейнера
		nodeAllocator* currentAllocator;//текущий контейнер

		//смена используемого в данный момент контейнера
		void changeCurrentAllocator()
		{
			for (auto i = localAllocatorSet.begin(); i != localAllocatorSet.end(); ++i)
			{
				if (getAllocator(*i)->freeSpace() > 256)
				{
					currentAllocatorNumber = *i;

					currentAllocator = getAllocator(*i);
					return;
				}
			}

			currentAllocatorNumber = getFreeAllocator();
			currentAllocator = getAllocator(currentAllocatorNumber);
			localAllocatorSet.push_back(currentAllocatorNumber);
		}//сменить текущий контейнер
		static bool isFirstThread;//первый терминал перезагружает хранилище

		nodeData* getNode(nodeId address)
		{
			return getAllocator(address.first)->get(address.second);
		}
		friend class nodePtr;
		friend class creator;

		nodeId allocate()
		{
			assert(currentAllocator);
			if (currentAllocator->freeSpace() <= static_cast<ushort>(256))
				changeCurrentAllocator();
			nodeId id(currentAllocatorNumber, currentAllocator->allocate());
			++numberOfNodes;
			return id;
		}
		void deallocate(nodeId id)
		{
			assert(getAllocator(id.exist()));
			getNode(id)->type = nodeT::Deleted;
			getAllocator(id.first)->deallocate(id.second);
			--numberOfNodes;
		}
	public:

		explicit threadLocalAllocator()
		{
			if (isFirstThread)
			{
				isFirstThread = false;
				reset();
			}
			changeCurrentAllocator();
		}
		~threadLocalAllocator()
		{
			for (auto i = localAllocatorSet.begin(); i != localAllocatorSet.end(); ++i)
			{
				toFreeAllocator(*i);//высвобождает используемые контейнеры, делая их доступными для других терминалов
			}
		}
		i64 numberOfNodes = 0; //количество нод
	};
	bool threadLocalAllocator::isFirstThread = true;
	thread_local threadLocalAllocator terminal;

}
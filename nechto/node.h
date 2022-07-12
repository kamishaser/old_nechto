#pragma once

#include "staticAllocator.h"

#include <memory>
#include <vector>
#include <atomic>
#include <mutex>
#include <thread>
#include <cassert>
#include <iostream>
#include <string>
#include <functional>
#include <compare>

class commandLine;
namespace nechto
{
	namespace nodeStorage
	{
		class Terminal;
	}
	
	using ushort = unsigned short;
	struct node //один узел nechto
 	{
		class ptr
		{
			ushort first;
			ushort second;

			ptr(ushort f, ushort s){
				first = f;
				second = s;
			}
			friend class nodeStorage::Terminal;
			friend class commandLine;
		public:
			ushort getFirst() const
			{
				return first;
			}
			ushort getSecond() const
			{
				return second;
			}
			ptr()
			{
				first = 0;
				second = 0;
			}
			bool exist() const
			{
				return first != 0;
			}
			operator bool() const
			{
				return exist();
			}
			bool isCorrect() const;
			
			node* operator-> ();
			node* operator* ();

			const node* operator-> () const;
			const node* operator* () const;

			/*bool operator<(const ptr& v2)const
			{
				const void* v1p = this;
				const void* v2p = &v2;
				return *reinterpret_cast<const uint32_t*>(v1p) < *reinterpret_cast<const uint32_t*>(v2p);
			}
			bool operator==(const ptr& v2)const
			{
				return (first == v2.first) && (second == v2.second);
			}
			bool operator!=(const ptr& v2)const
			{
				return (first != v2.first) || (second != v2.second);
			}*/
			auto operator<=>(const ptr&) const = default;
		};
	private:
		std::atomic<size_t> data = 0;//данные ноды
		std::atomic<char> type;//тип ноды
		std::atomic<char> subtype;//подтип ноды
		std::atomic<bool> correctnessСhecked = false;
	public:
		std::atomic<ptr> connection[4];
		std::atomic<ptr> hubConnection;
	
		friend void setTypeAndSubtype(ptr, char, char);
		

		template <class TCon>
		const TCon getData() const //получение данных в формате <TCon>
		{
			assert(sizeof(TCon) <= sizeof(size_t));
			size_t temp = data.load();
			return *static_cast<TCon*>(static_cast<void*>(&temp));
		}
		template <class TCon>
		void setData(TCon Data) //запись данных в формате TCon
		{
			assert(sizeof(TCon) <= sizeof(size_t));
			size_t temp = *static_cast<size_t*>(static_cast<void*>(&Data));
			data.store(temp);
		}

		bool hasConnection(int number) const //проверка наличия соединения по номеру	
		{
			assert(number < 4);
			return (connection[number].load());
		}
		bool hasHub() const //проверка наличия соединения по номеру	
		{
			return (hubConnection.load());
		}
		void correctnessHasNotBeenChecked()
		{
			correctnessСhecked = false;
		}
		int connectionType(int number) const //получение типа ноды подключённой по номеру
		{
			if (!hasConnection(number)) return 0;
			return connection[number].load()->getType();
		}
		int connectionSubtype(int number) const//получение подтипа ноды подключённой по номеру
		{
			if (!hasConnection(number)) return 0;
			return connection[number].load()->getSubtype();
		}
		char getType() const
		{
			return type;
		}
		char getSubtype() const 
		{
			return subtype;
		}
		enum Type //список типов нод
		{
			Error,
			Hub,					//разветвитель
			Variable,				//объект-переменная базового типа, хранящаяся внутри алгоритма (одинаков для всех исполнителей)
			TypeCastOperator,		//оператор преобразования типа данных
			MathOperator,			//математический оператор
			ConditionalBranching,	//if
			ExteralFunction,		//функция, не являющаяся частью nechto
			Tag,					//метка
			Pointer,					//указатель на объект
			Array
		};
		
	};

	using nodePtr = node::ptr;
	using nodeEvent = std::function<void(nodePtr)>;
	using nodeConnectionEvent = std::function<void(nodePtr, nodePtr)>;
	const nodePtr nullNodePtr = nodePtr(); //аналог nullptr

	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	//всё, что ниже этой строки тебе не понадобится

	namespace nodeStorage
	{
		const int maxNumOfAllocators = 16; //максимальное количество выделяемых аллокаторов
		static std::unique_ptr<staticAllocator<node>> content[maxNumOfAllocators]; //массив контейнеров
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
					content[occupancy[number]] = std::make_unique<staticAllocator<node>>();
					return number;
				}
				if (content[occupancy[number]]->freeSpace() > 256)
				{
					changeAllocatorBlock.unlock();
					return number;
				}
			}

		}

		staticAllocator<node>* getAllocator(const ushort number)
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

		class Terminal
		{//реализовано отдельным классом для корректной работы конструктора и деструктора
			std::vector<ushort> localAllocatorSet;//список номеров зарезервированных контейнеров
			ushort currentAllocatorNumber;//номер текущего контейнера
			staticAllocator<node>* currentAllocator;//текущий контейнер

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
			static bool isFistTerminal;//первый терминал перезагружает хранилище
		public:

			explicit Terminal()
			{
				if (isFistTerminal)
				{
					isFistTerminal = false;
					reset();
				}
				changeCurrentAllocator();
			}
			~Terminal()
			{
				for (auto i = localAllocatorSet.begin(); i != localAllocatorSet.end(); ++i)
				{
					toFreeAllocator(*i);//высвобождает используемые контейнеры, делая их доступными для других терминалов
				}
			}
			const nodePtr allocate()
			{
				assert(currentAllocator != nullptr);
				if (currentAllocator->freeSpace() <= static_cast<ushort>(256))
					changeCurrentAllocator();
				nodePtr id;
				id.first = currentAllocatorNumber;
				id.second = currentAllocator->allocate();
				return id;
			}
			void deallocate(const nodePtr id)
			{
				assert(getAllocator(id.first) != nullptr);
				getAllocator(id.first)->deallocate(id.second);
			}
		};
		bool Terminal::isFistTerminal = true;
		thread_local Terminal terminal;

	}
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	node* node::ptr::operator-> ()
	{
		assert(exist());
		return nodeStorage::getAllocator(first)->get(second);
	}
	node* node::ptr::operator* ()
	{
		assert(exist());
		return nodeStorage::getAllocator(first)->get(second);
	}

	const node* node::ptr::operator-> () const
	{
		assert(exist());
		return nodeStorage::getAllocator(first)->get(second);
	}
	const node* node::ptr::operator* () const
	{
		assert(exist());
		return nodeStorage::getAllocator(first)->get(second);
	}
}
#pragma once

#include "staticAllocator.h"

#include <memory>
#include <vector>
#include <atomic>
#include <mutex>
#include <thread>
#include <cassert>
#include <iostream>

namespace nechto
{
	using ushort = unsigned short;
	struct node //один узел nechto
 	{
		struct ptr
		{
			ushort first;
			ushort second;

			ptr(ushort f = 0, ushort s = 0){
				first = f;
				second = s;
			}
			ptr(std::pair<ushort, ushort> address)
			{
				first = address.first;
				second = address.second;
			}
			bool exist() const
			{
				return first != 0;
			}
			operator bool() const
			{
				return exist();
			}
			node* operator-> () const;
			node* operator* () const;
		};
		
		std::atomic<size_t> data = 0;
		std::atomic<ushort> type;
		std::atomic<ushort> subtype;
		std::atomic<ptr> connection[4];
		std::atomic<ptr> hubConnection;

		template <class TCon>
		const TCon getData() const
		{
			assert(sizeof(TCon) <= sizeof(size_t));
			size_t temp = data.load();
			return *static_cast<TCon*>(static_cast<void*>(&temp));
		}
		template <class TCon>
		void setData(TCon Data)
		{
			assert(sizeof(TCon) <= sizeof(size_t));
			size_t temp = *static_cast<size_t*>(static_cast<void*>(&Data));
			data.store(temp);
		}

		bool hasConnection(int number)
		{
			assert(number < 4);
			return (connection[number].load());
		}
		int connectionType(int number)
		{
			assert(hasConnection(number));
			return connection[number].load()->type;
		}
		int connectionSubtype(int number)
		{
			assert(hasConnection(number));
			return connection[number].load()->subtype;
		}

		enum Type
		{
			Empty,					//пустой объект
			Hub,					//разветвитель
			Variable,				//объект-переменна€ базового типа, хнан€ща€с€ внутри алгоритма (одинаков дл€ всех исполнителей)
			TypeCastOperator,		//оператор преобразовани€ типа данных
			MathOperator,			//математический оператор
			ConditionalBranching,	//if
			Pointer,				//указатель на объект
			Function,				//функци€, не €вл€юща€с€ частью nechto
			BranchingMerge,			//сли€ние ветвей
			Tag,					//помечнный извне объект (односторонн€€ св€зь может быть только к метке 
									//(о метке знает только один объект))
			TagCall,				//вызов алгоритма по тегу
			NumberOfTypes			//не объект. „исло, обозначающее количество типов
		};
		
	};

	using nodePtr = node::ptr;

	const nodePtr nullNodePtr(0, 0);

	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	const int maxNumOfAllocators = 65536;

	namespace nodeStorage
	{
		static_assert(maxNumOfAllocators <= 65536, "65536 is maximum");
		std::unique_ptr<staticAllocator<node>> content[maxNumOfAllocators]; //массив контейнеров
		ushort occupancy[maxNumOfAllocators]; //массив зан€тости
		std::atomic<ushort> sflag; //номер занимаемого контейнера
		std::atomic<ushort> freeSpace; //количество созданных контейнеров

		std::mutex changeAllocatorBlock;



		void toFreeAllocator(ushort number)//освобождение контейнера
		{
			changeAllocatorBlock.lock();
			occupancy[sflag.fetch_add(-1)] = number;
			++freeSpace;
			changeAllocatorBlock.unlock();
		}
		ushort getFreeAllocator()//вз€тие контейнера
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

		ushort getFreeSpace()
		{
			return freeSpace;
		}
		void reset()
		{//полный сброс хранилища. ќпасна€ операци€!
			changeAllocatorBlock.lock();
			for (int i = 0; i < maxNumOfAllocators; i++)
				occupancy[i] = i;
			sflag = 1;
			freeSpace = maxNumOfAllocators - 1;
			changeAllocatorBlock.unlock();
		}

		//////////////////////////////////////////////////////////////////////////////////////////

		class Terminal
		{//реализовано отдельным классом дл€ корректной работы конструктора и деструктора
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
					toFreeAllocator(*i);//высвобождает используемые контейнеры, дела€ их доступными дл€ других терминалов
				}
			}
			const nodePtr allocate()
			{
				assert(currentAllocator != nullptr);
				if (currentAllocator->freeSpace() <= static_cast<ushort>(256))
					changeCurrentAllocator();
				std::pair<ushort, ushort> id;
				id.first = currentAllocatorNumber;
				id.second = currentAllocator->allocate();
				return id;
			}
			void deallocate(const nodePtr id)
			{
				getAllocator(id.first)->deallocate(id.second);
			}
		};
		bool Terminal::isFistTerminal = true;
		thread_local Terminal terminal;

	}
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	node* node::ptr::operator-> () const
	{
		return nodeStorage::getAllocator(first)->get(second);
	}
	node* node::ptr::operator* () const
	{
		return nodeStorage::getAllocator(first)->get(second);
	}
}
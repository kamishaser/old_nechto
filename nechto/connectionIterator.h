#pragma once
#include "node.h"
#include "hub.h"

namespace nechto
{
	//не тестировал
	struct hubIterator //общий итератор цепочки хабов. Работает как массивами, так и с обычными цепочками
	{
		nodePtr currentHub;
		nodePtr mainNode;
		char position = 0;

		hubIterator() {}
		hubIterator(nodePtr curHub, nodePtr base, char pos)
			:currentHub(curHub), mainNode(base), position(pos) {}
		hubIterator(nodePtr v1)
			:currentHub(v1), mainNode(v1), position(0){}
		
		//элемент, на который указывает итератор
		nodePtr get() const
		{
			return currentHub->connection[pos()].load();
		}
		std::atomic<nodePtr>& getA()
		{
			return currentHub->connection[pos()];
		}
		//позиция итератора в хабе. Номер соединения от 0 до 3
		char pos() const
		{
			return position & static_cast<char>(3);
		}
		//вытянуть данные из ноды итератора
		void pull(nodePtr v1)
		{
			assert(typeCompare(v1, node::Pointer));
			assert(!subtypeCompare(v1, pointer::Reference));
			currentHub = v1->connection[0];
			mainNode = v1->connection[1];
			position = v1->getData<char>();
		}
		//отправить данные в ноду итератор
		void push(nodePtr v1)
		{
			assert(typeCompare(v1, node::Pointer));
			assert(!subtypeCompare(v1, pointer::Reference));
			v1->connection[0] = currentHub;
			v1->connection[1] = mainNode;
			v1->setData<char>(position);
		}
		//односторонне подключается к v1. Возвращает старое соединение
		nodePtr oneSideConnect(nodePtr v1)
		{
			return currentHub->connection[pos()].exchange(v1);
		}
		//односторонне отключается. Возвращает старое соединение
		nodePtr oneSideDisconnect()
		{
			return currentHub->connection[pos()].exchange(nullNodePtr);
		}
			
	};

	class connectionIterator : public hubIterator
	{
	public:
		bool stepForward()
		{
			++position;
			if ((position & 3ll) != 0)
				return true;
			else
			{
				nodePtr nextNode = currentHub->hubConnection.load();
				//если следующего хаба не существует - переход в основную ноду
				if (nextNode.exist())
				{
					currentHub = nextNode;
					return true;
				}
				else
				{
					currentHub = mainNode;
					position = 0;
					return false;
				}
			}
		}
		bool stepBack()
		{
			--position;
			if ((position & 3ll) != 3)
				return true;//если перемещение происходит в границах одного хаба - всё норм
			else
			{
				nodePtr previousNode =
					currentHub->getData<std::pair<nodePtr, nodePtr>>().first;
				//если  хаба не существует - переход в основную ноду
				if (previousNode.exist())
				{
					currentHub = previousNode;
					return true;
				}
				else
				{
					nodePtr hubIterator = currentHub;
					i64 pos = 3;
					while (true)
					{
						nodePtr next = hubIterator->hubConnection;
						if (!next.exist())
							break;
						++pos;
						hubIterator = next;
					}
					currentHub = hubIterator;
					position = pos;
					return false;
				}
			}
		}
		//итератор на первом элементе
		bool atFirst()
		{
			if (!pos())
				return false;
			if (currentHub != mainNode)
				return false;
			return true;
		}
		//итератор на последнем элементе
		bool atLast()
		{
			if (pos() != 3)
				return false;
			if (currentHub->hasHub())
				return false;
			return true;
		}
		//вставить хаб после текущего
		void insertHub()
		{
			hub::insert(currentHub, mainNode);
		}
		//удалить текущий хаб. Перевести все указывающие на него итераторы на следующий
		void eraseHub()
		{
			assert(currentHub != mainNode);
			//перебирает все подключённые ноды в поисках итератора, указывающего на удаляемый хаб
			connectionIterator conIter(mainNode);
			do
			{
				for (int i = 0; i < 4; ++i)
				{
					nodePtr temp = conIter.get();
					if (temp.exist())
						if (typeSubtypeCompare(temp, node::Pointer, pointer::ConIter))
						{//переводит на следующий
							nodePtr hub = temp->connection[0];
							if (hub == currentHub)
								temp->connection[0] = hub::next(hub);
						}
				}
			} while (conIter.stepForward());
			hub::erase(currentHub, mainNode);
			
		}
	};
	class arrayIterator
	{
	public:
		bool stepForward()
		{
			++position;
			if (!(position & 3ll))//если позиция == 0, переход в следующий хаб
				currentHub = currentHub->hubConnection.load();
		}
		void stepBack()
		{
			if (!(position & 3ll))//если позиция равна 0, переход в предыдущий хаб
				currentHub = currentHub->getData<std::pair<nodePtr, nodePtr>>().first;
			--position;
		}
		nodePtr mainNode = nullNodePtr;
		nodePtr currentHub = nullNodePtr;
		i64 position;

		nodePtr firstHub()
		{
			return mainNode->connection[0];
		}

		bool atFirst()
		{
			if (!(position & 3ll))
				return false;
			nodePtr begin = firstHub();
			if (currentHub != begin->connection[0])
				return false;
			return true;
		}
		bool insertHub()
		{
			mainNode->data.fetch_add(1);
			hub::insert(currentHub, mainNode);
		}
		void eraseHub()
		{
			assert(currentHub != mainNode);
			if (mainNode->data.fetch_add(-1) == 1)
			{//нельзя удалять последний хаб
				mainNode->data.fetch_add(1);
				return;
			}
			//перебирает все подключённые ноды в поисках итератора, указывающего на удаляемый хаб
			connectionIterator conIter(mainNode);
			do
			{
				for (int i = 0; i < 4; ++i)
				{
					nodePtr temp = conIter.get();
					if (temp.exist())
						if (typeSubtypeCompare(temp, node::Pointer, pointer::ArrayIter))
						{//переводит на следующий
							nodePtr hub = temp->connection[0];
							if (hub == currentHub)
								temp->connection[0] = hub::next(hub);
						}
				}
			} while (conIter.stepForward());
			hub::erase(currentHub, mainNode);
		}
	};
	/*обменивает места подключения по одной стороне, 
	возвращает true, если обмен удался*/
	bool swap(hubIterator& i1, hubIterator& i2)
	{
		if (i1.mainNode != i2.mainNode)
			return false;
		i1.getA().exchange(i2.getA());
	}
}
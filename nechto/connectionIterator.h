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
		char position = 0;//внимание: считыть только через pos()

		hubIterator() {}
		hubIterator(nodePtr curHub, nodePtr base, char pos)
			:currentHub(curHub), mainNode(base), position(pos) {}
		
		//элемент, на который указывает итератор
		/*const std::atomic<nodePtr>* operator->() const
		{
			return currentHub->connection[pos()];
		}
		const std::atomic<nodePtr>* operator*() const
		{
			return currentHub->connection[pos()];
		}*/
		std::atomic<nodePtr>& operator->()
		{
			return currentHub->connection[pos()];
		}
		std::atomic<nodePtr>& operator*()
		{
			return currentHub->connection[pos()];
		}
		nodePtr get()const 
		{
			return currentHub->connection[pos()].load();
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
		void push(nodePtr v1) const
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
			return oneSideConnect(nullNodePtr);
		}
		bool operator==(const hubIterator& i2) const
		{
			return (mainNode == i2.mainNode && pos() == i2.pos() &&
				currentHub == i2.currentHub);
		}
		bool operator!=(const hubIterator& i2) const
		{
			return (mainNode != i2.mainNode || pos() != i2.pos() ||
				currentHub != i2.currentHub);
		}
		//обмен односторонних соединений в пределах одной цепочки
		static bool exchangeConnections(hubIterator& fI, hubIterator& sI)
		{
			if (fI.mainNode != sI.mainNode)
				return false;
			nodePtr temp = fI.get();
			fI.currentHub->connection[fI.pos()] = sI.currentHub->connection[sI.pos()].load();
			sI.currentHub->connection[sI.pos()] = temp;
			return true;
		}
	};

	class connectionIterator : public hubIterator
	{
	public:
		connectionIterator(nodePtr v1)
			:hubIterator(v1, v1, 0) {}
		bool GoToNextHub()
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
		bool GoToPreviousHub()
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

		bool stepForward()
		{
			++position;
			if (!(position & 3ll))
				return true;
			else
				return GoToNextHub();
		}
		bool stepBack()
		{
			--position;
			if ((position & 3ll) != 3)
				return true;//если перемещение происходит в границах одного хаба - всё норм
			else
				return GoToPreviousHub();
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
	class groupIterator : public hubIterator
	{
	public:
		groupIterator(nodePtr v1)
			:hubIterator(v1->connection[0], v1, 0) 
		{
			assert(typeCompare(v1, node::Group));
		}

		bool GoToNextHub()
		{
			currentHub = currentHub->hubConnection.load();
			return (currentHub != mainNode->connection[0]);
		}
		bool GoToPreviousHub()
		{
			bool result = currentHub != mainNode->connection[0];
			currentHub = currentHub->getData<std::pair<nodePtr, nodePtr>>().first;
			return result;
		}
		bool stepForward()
		{
			++position;
			if (!(position & 3ll))//если позиция == 0, переход в следующий хаб
			{
				return GoToNextHub();
			}
			else
				return true;
		}
		bool stepBack()
		{
			bool result;
			if (!(position & 3ll))//если позиция равна 0, переход в предыдущий хаб
				result = GoToPreviousHub();
			else
				result = true;
			--position;
			return result;
		}
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
		void insertHub()
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
						if (typeSubtypeCompare(temp, node::Pointer, pointer::GroupIter))
						{//переводит на следующий
							nodePtr hub = temp->connection[0];
							if (hub == currentHub)
								temp->connection[0] = hub::next(hub);
						}
				}
			} while (conIter.stepForward());
			if (mainNode->connection[0] = currentHub)
				mainNode->connection[0] = hub::next(currentHub);
			hub::erase(currentHub, mainNode);
		}
	};
}
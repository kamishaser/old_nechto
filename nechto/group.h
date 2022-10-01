#pragma once
#include "node.h"
#include "pointer.h"
#include "connectionIterator.h"

namespace nechto
{
	namespace group
	{
		void initializeEmpty(nodePtr v1)
		{
			//первый хаб массива. (замкнут сам на себя)
			nodePtr hub = newNode(node::Hub);
			hub->hubConnection = hub;
			hub->setData(std::pair<nodePtr, nodePtr>(hub, v1));

			v1->setData<nodePtr>(hub);
		}
		void reset(nodePtr v1)
		{
			groupIterator i1(v1);
			while (true)
			{
				nodePtr connection = i1.oneSideDisconnect();
				if (connection.exist())
				{
					//при удалении ноды надо удалить все существующие соединения
					if (typeSubtypeCompare(
						connection, node::Pointer, pointer::GroupIter) &&
						(connection->connection[0] == v1))
					{
						connection->connection[0] = nullNodePtr;
						connection->setData<pointer::hubPosPair>(
							pointer::hubPosPair(nullNodePtr, 0));

					}
					else
						oneSideDisconnect(connection, v1);
				}
				if (i1.pos() == 3)
				{
					nodePtr vTemp = i1.currentHub;
					bool end = !i1.GoToNextHub();
					nodeStorage::terminal.deallocate(vTemp);
					i1.setLocalPos(0);
					if (end)
					{
						assert(typeCompare(i1.currentHub, node::Deleted));
						return;
					}
				}
				else
					i1.stepForward();
			}
		}
		i64 numberOfMembers(nodePtr group)
		{
			i64 counter = 0;
			groupIterator i(group);
			do
			{
				if (i.get().exist())
					++counter;
			} while (i.stepForward());
			return counter;
		}
		bool empty(nodePtr group)
		{
			groupIterator i(group);
			do
			{
				if (i.get().exist())
					return false;
			} while (i.stepForward());
			return true;
		}

		void clear(nodePtr v1)
		{
			reset(v1);
			initializeEmpty(v1);
		}
		bool contains(nodePtr group, nodePtr v1)
		{
			groupIterator gi(group);
			do
			{
				if (gi.get() == v1)
					return true;
			} while (gi.stepForward());
			return false;
		}
		groupIterator getIteratorToConnection(nodePtr group, nodePtr v1)
		{
			groupIterator gi(group);
			do
			{
				if (gi.get() == v1)
					return gi;
			} while (gi.stepForward());
			return groupIterator(nullNodePtr, pointer::hubPosPair(nullNodePtr, 0));
		}
		bool check(nodePtr v1)
		{
			assert(v1->getType() == node::Group);
			if (v1->data < 1)
				return false;
			//nodePtr hubIter = v1->connection[0];

			//if (!v1->getType())
			return true;
		}
		
		void compress(const groupIterator& begin)
		{
			groupIterator pullIter = begin;
			groupIterator pushIter = begin;
			do
			{
				nodePtr temp = *pullIter;
				*pullIter = nullNodePtr;//надо переделать для оптимизации
				if (temp.exist())
				{
					*pushIter = temp;
					pushIter.stepForward();
				}
			} while (pullIter.stepForward());
			if (pushIter.pos() != 0)
				pushIter.stepForward();
			while (pushIter.currentHub != pushIter.mainNode->connection[0])
			{
				assert(hub::empty(pushIter.currentHub));
				pushIter.eraseHub();
			}
		}
		void compress(nodePtr v1)
		{
			assert(v1.exist());
			assert(typeCompare(v1, node::Group));
			compress(groupIterator(v1));			
		}
		groupIterator lastConnectedPort(nodePtr group)
		{
			assert(group.exist());
			assert(typeCompare(group, node::Group));
			groupIterator i1(group);
			do
			{
				i1.stepBack();
				if (i1.get().exist())
					return i1;
			} while (i1.stepBack());
			return i1;//если пустых элементов нет, возвращает итератор на первый элемент
		}
		groupIterator firstEmptyPort(nodePtr group)
		{
			assert(group.exist());
			assert(typeCompare(group, node::Group));
			groupIterator i1(group);
			do
			{
				if (!i1.get().exist())
					return i1;
			} while (i1.stepForward());
			i1.stepBack();//костыль. Переделать когда отрефакторю вставку хабов в группу
			i1.insertHub();
			i1.stepForward();
			return i1;
		}
		nodePtr getFirstHub(nodePtr group)
		{
			return group->getData<nodePtr>();
		}
	}
}
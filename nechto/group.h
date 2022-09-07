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
			nodePtr hubIter = v1->getData<nodePtr>();
			for (i64 i = 0; i < v1->data; ++i)
			{
				hub::deleteAllConnectionsInHub(hubIter, v1);
				nodePtr next = hubIter->hubConnection;
				deleteNode(hubIter);
				hubIter = next;
			}
			hubIter->connection[0] = nullNodePtr;
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
	}
}
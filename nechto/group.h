#pragma once
#include "node.h"
#include "pointer.h"
#include "connectionIterator.h"

namespace nechto
{
	namespace group
	{
		//к основной ноде массива подключена замкнутая цепочка хабов [0]
		//в data хранится размер массива

		void initializeEmpty(nodePtr v1)
		{
			//первый хаб массива. (замкнут сам на себя)
			nodePtr hub = newNode(node::Hub);
			hub->hubConnection = hub;
			hub->setData(std::pair<nodePtr, nodePtr>(hub, v1));

			v1->setData<i64>(1);
			v1->connection[0] = hub;
		}
		void reset(nodePtr v1)
		{
			nodePtr hubIter = v1->connection[0];
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
		//void copy(nodePtr v1);
		void compress(nodePtr v1)
		{
			assert(v1.exist());
			assert(typeCompare(v1, node::Group));
			groupIterator pullIter(v1);
			groupIterator pushIter(v1);

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
			while (pushIter.currentHub != v1->connection[0])
			{
				assert(hub::empty(pushIter.currentHub));
				pushIter.eraseHub();
			}
		}
	}
}
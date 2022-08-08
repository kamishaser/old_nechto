#pragma once
#include "node.h"
#include "nodeOperations.h"
#include "pointer.h"

namespace nechto
{
	namespace array
	{
		//к основной ноде массива подключена замкнутая цепочка хабов [0]
		//в data хранится размер массива

		void initializeEmpty(nodePtr array)
		{
			//первый хаб массива. (замкнут сам на себя)
			nodePtr hub = newNode(node::Hub);
			hub->hubConnection = hub;
			hub->setData(std::pair<nodePtr, nodePtr>(hub, array));

			array->setData<i64>(1);
			array->connection[0] = hub;
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
			assert(v1->getType() == node::Array);
			if (v1->data < 1)
				return false;
			//nodePtr hubIter = v1->connection[0];

			//if (!v1->getType())
			return true;
		}
		//void copy(nodePtr v1);

	}
}
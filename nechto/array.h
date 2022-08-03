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

		void perform(nodePtr v1);
		bool check(nodePtr v1);
		void copy(nodePtr v1);
	}
}
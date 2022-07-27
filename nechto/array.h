#pragma once
#include "node.h"
#include "nodeOperations.h"
#include "pointer.h"

namespace nechto
{
	namespace array
	{
		//каждый массив имеет следующую структуру:
		//основная нода массива к которой подключены 2 ноды итераторов: begin, end
		//который в свою очередь указывают на цепочку хабов

		void initializeEmpty(nodePtr array, char subtype)
		{
			nodePtr begin = newNode(node::Pointer, subtype);
			nodePtr end = newNode(node::Pointer, subtype);
			NumConnect(array, begin, 0);
			NumConnect(array, end, 0);
		}
		void reset(nodePtr v1);
		void perform(nodePtr v1);
		bool check(nodePtr v1);
		void copy(nodePtr v1);
	}
}
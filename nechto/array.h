#pragma once
#include "node.h"
#include "nodeOperations.h"
#include "pointer.h"

namespace nechto
{
	namespace array
	{
		//� �������� ���� ������� ���������� ��������� ������� ����� [0]
		//� data �������� ������ �������

		void initializeEmpty(nodePtr array)
		{
			//������ ��� �������. (������� ��� �� ����)
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
#pragma once
#include "node.h"
#include "nodeOperations.h"
#include "pointer.h"

namespace nechto
{
	namespace array
	{
		//� �������� ���� ������� ���������� 2 ���� ����������: begin, end (0, 1)
		//������� ��������� �� ������� �����

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
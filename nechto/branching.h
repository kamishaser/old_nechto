#pragma once
#include "node.h"
#include "mathOperator.h"
#include "pointer.h"
#include "connectionIterator.h"

namespace nechto
{}
//�������� ���������
//� ������ ������ nechto ������ �������� ���������
namespace nechto::branching
{
	//� ���������� 0 ������������ ������� (�������� ���� ����� ���������)
	//� ������������ 1 � 2 ��������� �������� (true, false)
	
	bool check(nodePtr v1)
	{
		assert(typeCompare(v1, node::ConditionalBranching));
		nodePtr condition = v1->connection[0];
		if (!condition.exist())
			return false;
		if (condition->getType() == node::Pointer)
		{//������ � ������� �� ��������, � ����� ���������
			condition == pointer::follow(condition);
			if (!condition.exist())
				return false;
		}
		if (condition->getType() != node::Variable)
			return false;
		if (!condition->getSubtype())//� ������� ������ int
			return false;
		return true;
	}
	//���������� ���������. ���������� 0, ���� ������, ����� ����� ��������� ����
	char perform(nodePtr v1)
	{
		nodePtr condition = v1->connection[0];
		if (condition->getType() == node::Pointer)
		{//������ � ������� �� ��������, � ����� ���������
			condition == pointer::follow(condition);
			if (!condition.exist())
				return 0;
		}
		if (condition->getType() != node::Variable)
			return 0;
		if (!condition->getSubtype())//� ������� ������ int
			return 0;
		return condition->data;
	}
}
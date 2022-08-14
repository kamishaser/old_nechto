#pragma once
#include "node.h"
#include "text.h"
#include "mathOperator.h"
#include "nodeOperations.h"
#include "externalFunction.h"
#include "Pointer.h"
#include "branching.h"

namespace nechto
{
	bool isAction(nodePtr v1)
	{
		switch (v1->getType())
		{
		case node::MathOperator:
		case node::ConditionalBranching:
		case node::ExternalFunction:
			return true;
		default:
			return false;
		}
	}
	bool nodePtr::check() const
	{
		nodePtr temp = *this;
		if(temp->correctness�hecked.load())
			return true;
		switch (temp->getType())
		{
		case node::Deleted://���� ����������� ���� �� ���������� - ���-�� �� ���
			temp->correctness�hecked = false;
			break;
		case node::Hub://���� ������ �� ����� �����������.
			temp->correctness�hecked = false;
			break;
		case node::Variable://���������� �� ����� ����������� �����������. 
			temp->correctness�hecked = true;
			break;
		case node::MathOperator:
			temp->correctness�hecked = mathOperator::check(*this);
			break;
		case node::Text:
			temp->correctness�hecked = text::check(*this);
			break;
		case node::ConditionalBranching:
			temp->correctness�hecked = branching::check(*this);
			break;
		case node::ExternalFunction:
			temp->correctness�hecked = externalFunction::check(*this);
			break;
		case node::Pointer:
			temp->correctness�hecked = pointer::check((*this));
			break;
		default:
			assert(false);
		}
		return temp->correctness�hecked;
	}
	
	nodePtr step(nodePtr v1)
	{
		assert(v1.check());
		assert(isAction(v1));
		nodePtr nextPosition;
		char temp;
		switch (v1->getType())
		{
		case node::MathOperator:
			assert(mathOperator::perform(v1));
			nextPosition = v1->connection[3].load();
			if (!nextPosition.exist())
				return nullNodePtr;
			return nextPosition;
		case node::ConditionalBranching:
			temp = branching::perform(v1);
			assert(!temp);
			nextPosition = v1->connection[temp];
			return nextPosition;
		case node::ExternalFunction:
			assert(externalFunction::perform(v1));
			nextPosition = v1->connection[3].load();
			if (!nextPosition.exist())
				return nullNodePtr;
			return nextPosition;
		default: assert(false);
		}
	}
}
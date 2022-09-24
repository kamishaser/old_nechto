#pragma once
#include "node.h"
#include "text.h"
#include "mathOperator.h"
#include "nodeOperator.h"
#include "method.h"
#include "Pointer.h"
#include "branching.h"
#include "group.h"

namespace nechto
{
	bool isAction(nodePtr v1)
	{
		switch (v1->getType())
		{
		case node::MathOperator:
		case node::ConditionalBranching:
		case node::Method:
			return true;
		default:
			return false;
		}
	}
	bool check(nodePtr v1)
	{
		switch (v1->getType())
		{
		case node::Deleted://если проверяемой ноды не существует - что-то не так
			return false;
			break;
		case node::Hub://хабы вообще не здесь проверяются.
			return false;
			break;
		case node::Variable://переменная не имеет собственных подключений. 
			return true;
			break;
		case node::MathOperator:
			return mathOperator::check(v1);
			break;
		case node::Text:
			return text::check(v1);
			break;
		case node::ConditionalBranching:
			return branching::check(v1);
			break;
		case node::Method:
			return method::check(v1);
			break;
		case node::Pointer:
			return pointer::check((v1));
			break;
		case node::Group:
			return group::check(v1);
			break;
		case node::NodeOperator:
			return checkNodeOperator(v1);
		default:
			assert(false);
		}
	}
	
	nodePtr step(nodePtr v1)
	{
		assert(check(v1));
		assert(isAction(v1));
		nodePtr nextPosition = v1->connection[3].load();
		char temp;
		switch (v1->getType())
		{
		case node::MathOperator:
			assert(mathOperator::perform(v1));
			return nextPosition;
		case node::ConditionalBranching:
			temp = branching::perform(v1);
			assert(temp != 0);
			nextPosition = v1->connection[temp];
			return nextPosition;
		case node::Method:
			assert(method::operate(v1));
			return nextPosition;
		case node::NodeOperator:
			if (!operateNodeOperator(v1))
				assert(false);
			return nextPosition;
		default: assert(false);
		}
		return nullNodePtr;
	}
}
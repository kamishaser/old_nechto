#pragma once
#include "node.h"
#include "text.h"
#include "mathOperator.h"
#include "method.h"
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
		default:
			assert(false);
		}
	}
	
	nodePtr step(nodePtr v1)
	{
		assert(check(v1));
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
		case node::Method:
			assert(method::operate(v1));
			nextPosition = v1->connection[3].load();
			if (!nextPosition.exist())
				return nullNodePtr;
			return nextPosition;
		default: assert(false);
		}
		return nullNodePtr;
	}
}
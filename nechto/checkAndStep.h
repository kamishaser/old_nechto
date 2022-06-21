#pragma once
#include "node.h"
#include "tag.h"
#include "mathOperator.h"
#include "baseValueTypes.h"
#include "lowLevelGraphOperations.h"
#include "externalFunction.h"

namespace nechto
{
	bool isAction(nodePtr v1)
	{
		switch (v1->type.load())
		{
		case node::MathOperator:
		case node::TypeCastOperator:
		case node::ConditionalBranching:
		case node::ExteralFunction:
			return true;
		default:
			return false;
		}
	}
	bool isCorrect(nodePtr v1)
	{
		if (!v1.exist())
			return false;
		switch (v1->type.load())
		{
		case node::Error:
			return false;
		case node::Hub:
			return getHubParrent(v1).exist();
		case node::Variable:
			return (!v1->hasConnection(0) && !v1->hasConnection(1)
				 && !v1->hasConnection(2) && !v1->hasConnection(3)
				 && v1->subtype.load() != baseValueType::Error);
		case node::TypeCastOperator:
			return isTypeCastOperatorCorrect(v1);
		case node::MathOperator:
			return mathOperator::isCorrect(v1);
		case node::Tag:
			return tag::isCorrect(v1);
		case node::ConditionalBranching:
			return ((v1->hasConnection(0)) && (v1->connection[0].load()->type == node::Variable));
		case node::ExteralFunction:
			if (v1->getData<externalFunction*>() == nullptr)
				return false;
			if (v1->getData<externalFunction*>()->isCorrect(v1))
				return true;
		default:
			return false;
		}
	}
	
	bool step(nodePtr& flag)
	{
		assert(isCorrect(flag));
		assert(isAction(flag));
		nodePtr nextPosition;
		switch (flag->type)
		{
		case node::MathOperator:
			mathOperator::mathOperation(flag);
			nextPosition = flag->connection[3].load();
			if (!nextPosition.exist())
				return false;
			flag = nextPosition;
			return true;
		case node::TypeCastOperator:
			typeCast(flag);
			nextPosition = flag->connection[3].load();
			if (!nextPosition.exist())
				return false;
			flag = nextPosition;
			return true;
		case node::ConditionalBranching:
			nextPosition = (boolCast(flag->connection[0])) ? flag->connection[1] : flag->connection[2];
			if (!nextPosition.exist())
				return false;
			flag = nextPosition;
			return true;
		case node::ExteralFunction:
			(flag->getData<externalFunction*>())->Func(flag);
			nextPosition = flag->connection[3].load();
			if (!nextPosition.exist())
				return false;
			flag = nextPosition;
			return true;
		default: throw;
		}
	}
}
#pragma once
#include "node.h"
#include "tag.h"
#include "mathOperator.h"
#include "baseValueTypes.h"
#include "nodeOperations.h"
#include "externalFunction.h"
#include "Pointer.h"

namespace nechto
{
	bool isAction(nodePtr v1)
	{
		switch (v1->getType())
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
	bool nodePtr::isCorrect() const
	{
		nodePtr temp = *this;
		if (temp->correctnessÑhecked.load())
			return true;
		switch (temp->getType())
		{
		case node::Error:
			temp->correctnessÑhecked = false;
			break;
		case node::Hub:
			temp->correctnessÑhecked = getHubParrent(*this).exist();
			break;
		case node::Variable:
			temp->correctnessÑhecked = (!temp->hasConnection(0) && !temp->hasConnection(1)
				 && !temp->hasConnection(2) && !temp->hasConnection(3)
				 && temp->getSubtype() != baseValueType::Error);
			break;
		case node::TypeCastOperator:
			temp->correctnessÑhecked = isTypeCastOperatorCorrect(*this);
			break;
		case node::MathOperator:
			temp->correctnessÑhecked = mathOperator::isCorrect(*this);
			break;
		case node::Tag:
			temp->correctnessÑhecked = tag::isCorrect(*this);
			break;
		case node::ConditionalBranching:
			temp->correctnessÑhecked = ((temp->hasConnection(0)) && (temp->connection[0].load()->getType() == node::Variable));
			break;
		case node::ExteralFunction:
			if (temp->getData<externalFunction*>() == nullptr)
				temp->correctnessÑhecked = false;
			if (temp->getData<externalFunction*>()->isCorrect((*this)))
				temp->correctnessÑhecked = true;
			break;
		case node::Pointer:
			temp->correctnessÑhecked = pointer::isCorrect((*this));
			break;
		default:
			temp->correctnessÑhecked = false;
		}
		return temp->correctnessÑhecked;
	}
	
	nodePtr step(nodePtr flag)
	{
		assert(flag.isCorrect());
		assert(isAction(flag));
		nodePtr nextPosition;
		switch (flag->getType())
		{
		case node::MathOperator:
			mathOperator::mathOperation(flag);
			nextPosition = flag->connection[3].load();
			if (!nextPosition.exist())
				return nullNodePtr;
			return nextPosition;
		case node::TypeCastOperator:
			typeCast(flag);
			nextPosition = flag->connection[3].load();
			if (!nextPosition.exist())
				return nullNodePtr;
			return nextPosition;
		case node::ConditionalBranching:
			nextPosition = (boolCast(flag->connection[0])) ? flag->connection[1] : flag->connection[2];
			if (!nextPosition.exist())
				return nullNodePtr;
			return nextPosition;
		case node::ExteralFunction:
			(flag->getData<externalFunction*>())->perform(flag);
			nextPosition = flag->connection[3].load();
			if (!nextPosition.exist())
				return nullNodePtr;
			return nextPosition;
		default: throw;
		}
	}
}
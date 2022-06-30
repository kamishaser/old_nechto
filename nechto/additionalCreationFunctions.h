#pragma once
#include "nodeOperations.h"
#include "checkAndStep.h"
#include "tag.h"
#include <type_traits>

namespace nechto
{
	nodePtr createNode(char type, char subtype)
	{
		nodePtr temp = newNode();
		setTypeAndSubtype(temp, type, subtype);
		return temp;
	}
	nodePtr createFunction(externalFunction* Func)
	{
		nodePtr temp = newNode();
		setTypeAndSubtype(temp, node::ExteralFunction);
		temp->setData(Func);
		return temp;
	}
	nodePtr createTypeCastOperator(nodePtr from, nodePtr to)
	{
		nodePtr temp = newNode();
		assert(from->getType() == node::Variable);
		assert(to->getType() == node::Variable);
		setTypeAndSubtype(temp, node::TypeCastOperator);
		NumHubConnect(temp, from, 1);
		NumHubConnect(temp, to, 0);
		return temp;
	}
	nodePtr createMathOperator(char subtype, nodePtr result,
		nodePtr first = nullNodePtr, nodePtr second = nullNodePtr)
	{
		nodePtr temp = newNode();
		setTypeAndSubtype(temp, node::MathOperator, subtype);
		assert(result->getType() == node::Variable);
		if (first.exist()) assert(first->getType() == node::Variable);
		if (second.exist())assert(second->getType() == node::Variable);
		NumHubConnect(temp, result, 0);
		if(first.exist())  NumHubConnect(temp, first, 1);
		if(second.exist()) NumHubConnect(temp, second, 2);
		return temp;
	}
	nodePtr createConditionalBranching(nodePtr condition,
		nodePtr truthWay = nullNodePtr, nodePtr wrongWay = nullNodePtr)
	{
		nodePtr temp = newNode();
		setTypeAndSubtype(temp, node::ConditionalBranching);
		assert(condition->getType() == node::Variable);
		NumHubConnect(temp, condition, 0);
		if (truthWay.exist())NumHubConnect(temp, truthWay, 1);
		if (wrongWay.exist())NumHubConnect(temp, wrongWay, 2);
		return temp;
	}
	nodePtr createExternalFunction(std::string funName)
	{
		if (!externalFunction::exist(funName))
			return nullNodePtr;
		nodePtr temp = newNode();
		setTypeAndSubtype(temp, node::ExteralFunction);
		temp->setData(externalFunction::get(funName));
		return temp;
	}

	void serialConnection(nodePtr first, nodePtr second, bool conditiion = true)
	{
		assert(isAction(first));
		assert(isAction(second));

		if (first->getType() != node::ConditionalBranching)
			NumHubConnect(first, second, 3);
		else if (conditiion)
			return NumHubConnect(first, second, 1);
		else
			NumHubConnect(first, second, 2);
	}

	template<class TCon>
	nodePtr createVariable(TCon startValue)
	{
		nodePtr temp = newNode();
		if (std::is_same<TCon, bool>())
		{
			setTypeAndSubtype(temp, node::Variable, baseValueType::Int64);
			temp->setData(static_cast<int64_t>(startValue));
			
			return temp;
		}
		if (std::is_same<TCon, long>())
		{
			assert(sizeof(TCon) == 4);
			
			setTypeAndSubtype(temp, node::Variable, baseValueType::Int64);
			temp->setData(static_cast<int64_t>(startValue));
			return temp;
		}
		if (std::is_same<TCon, int>())
		{
			assert(sizeof(TCon) == 4);
			setTypeAndSubtype(temp, node::Variable, baseValueType::Int64);
			temp->setData(static_cast<int64_t>(startValue));
			return temp;
		}
		if (std::is_same<TCon, int64_t>())
		{
			setTypeAndSubtype(temp, node::Variable, baseValueType::Int64);
			temp->setData(static_cast<int64_t>(startValue));
			return temp;
		}
		if (std::is_same<TCon, size_t>())
		{
			setTypeAndSubtype(temp, node::Variable, baseValueType::Int64);
			temp->setData(static_cast<int64_t>(startValue));
			return temp;
		}
		if (std::is_same<TCon, float>())
		{
			setTypeAndSubtype(temp, node::Variable, baseValueType::Double);
			temp->setData(static_cast<double>(startValue));
			return temp;
		}
		if (std::is_same<TCon, double>())
		{
			setTypeAndSubtype(temp, node::Variable, baseValueType::Double);
			temp->setData(static_cast<double>(startValue));
			return temp;
		}
		throw;
	}

}
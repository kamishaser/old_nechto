#pragma once
#include "lowLevelGraphOperations.h"
#include "checkAndStep.h"
#include "tag.h"
#include <type_traits>

namespace nechto
{
	nodePtr createNode(ushort type, ushort subtype)
	{
		nodePtr temp = newNode();
		temp->type = type;
		temp->subtype = subtype;
		return temp;
	}
	nodePtr createFunction(function* Func)
	{
		nodePtr temp = newNode();
		temp->type = node::Function;
		temp->setData(Func);
		return temp;
	}
	nodePtr createTypeCastOperator(nodePtr from, nodePtr to)
	{
		nodePtr temp = newNode();
		assert(from->type.load() == node::Variable);
		assert(to->type.load() == node::Variable);
		temp->type = node::TypeCastOperator;
		NumHubConnect(temp, from, 1);
		NumHubConnect(temp, to, 0);
		return temp;
	}
	nodePtr createMathOperator(ushort subtype, nodePtr result,
		nodePtr first = nullNodePtr, nodePtr second = nullNodePtr)
	{
		nodePtr temp = newNode();
		temp->type = node::MathOperator;
		temp->subtype = subtype;
		assert(result->type.load() == node::Variable);
		if (first.exist()) assert(first->type.load() == node::Variable);
		if (second.exist())assert(second->type.load() == node::Variable);
		NumHubConnect(temp, result, 0);
		if(first.exist())  NumHubConnect(temp, first, 1);
		if(second.exist()) NumHubConnect(temp, second, 2);
		return temp;
	}
	nodePtr createConditionalBranching(nodePtr condition,
		nodePtr truthWay = nullNodePtr, nodePtr wrongWay = nullNodePtr)
	{
		nodePtr temp = newNode();
		temp->type = node::ConditionalBranching;
		assert(condition->type.load() == node::Variable);
		NumHubConnect(temp, condition, 0);
		if (truthWay.exist())NumHubConnect(temp, truthWay, 1);
		if (wrongWay.exist())NumHubConnect(temp, wrongWay, 2);
		return temp;
	}

	void serialConnection(nodePtr first, nodePtr second, bool conditiion = true)
	{
		assert(isAction(first));
		assert(isAction(second));

		if (first->type != node::ConditionalBranching)
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
		temp->type = node::Variable;
		if (std::is_same<TCon, bool>())
		{
			temp->subtype.store(baseValueType::Int64);
			temp->setData<int64_t>(startValue);
			return temp;
		}
		if (std::is_same<TCon, long>())
		{
			assert(sizeof(TCon) == 4);
			temp->subtype.store(baseValueType::Int64);
			temp->setData<int64_t>(startValue);
			return temp;
		}
		if (std::is_same<TCon, int>())
		{
			assert(sizeof(TCon) == 4);
			temp->subtype.store(baseValueType::Int64);
			temp->setData<int64_t>(startValue);
			return temp;
		}
		if (std::is_same<TCon, int64_t>())
		{
			temp->subtype.store(baseValueType::Int64);
			temp->setData<int64_t>(startValue);
			return temp;
		}
		if (std::is_same<TCon, size_t>())
		{
			temp->subtype.store(baseValueType::Int64);
			temp->setData<int64_t>(startValue);
			return temp;
		}
		if (std::is_same<TCon, float>())
		{
			temp->subtype.store(baseValueType::Float);
			temp->setData(startValue);
			return temp;
		}
		if (std::is_same<TCon, double>())
		{
			temp->subtype.store(baseValueType::Double);
			temp->setData(startValue);
			return temp;
		}
		throw;
	}

}
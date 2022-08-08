#pragma once
#include "nodeOperations.h"
#include "checkAndStep.h"

namespace nechto
{
	class creationInCode
	{
	public:
		std::set<nodePtr> nodeSet;//набор созданных нод
		std::shared_ptr<externalFunction::externalFunctionSet> exFunSet;
		std::map<std::wstring, nodePtr> taggetNodeMap;//набор помеченных нод

		nodePtr createNode(char type, char subtype = 0, i64 data = 0)
		{
			nodePtr v1 = newNode(type, subtype);
			nodeSet.insert(v1);
			return v1;
		}
		nodePtr createMathOperator(char subtype, nodePtr result,
			nodePtr first = nullNodePtr, nodePtr second = nullNodePtr)
		{
			nodePtr temp = newNode(node::MathOperator, subtype);
			NumHubConnect(temp, result, 0);
			if (first.exist())  NumHubConnect(temp, first, 1);
			if (second.exist()) NumHubConnect(temp, second, 2);
			return temp;
		}
		nodePtr createConditionalBranching(nodePtr condition,
			nodePtr truthWay = nullNodePtr, nodePtr wrongWay = nullNodePtr)
		{
			nodePtr temp = newNode(node::ConditionalBranching);
			NumHubConnect(temp, condition, 0);
			if (truthWay.exist())NumHubConnect(temp, truthWay, 1);
			if (wrongWay.exist())NumHubConnect(temp, wrongWay, 2);
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
			nodePtr temp;
			if (std::is_same<TCon, bool>() ||
				std::is_same<TCon, long>() ||
				std::is_same<TCon, int>() ||
				std::is_same<TCon, i64>() ||
				std::is_same<TCon, size_t>())
			{
				temp = newNode(node::Variable, 1);
				temp->data = 0;
				return temp;
			}
			if (std::is_same<TCon, float>() ||
				std::is_same<TCon, f64>())
			{
				temp = newNode(node::Variable, 0);
				temp->setData(static_cast<f64>(startValue));
				return temp;
			}
			assert(false);
		}
		
	
	};
}
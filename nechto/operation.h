#pragma once
#include "conRuleInterface.h"
namespace nechto
{
	struct operationProperties
	{
		bool cangeNet = false;
	};
	struct operationArgument
	{
		nodePtr operationNode;
		nodePtr port0;
		nodePtr port1;
		nodePtr port2;
	};
	struct operation
	{
		bool(*operationPtr)(const operationArgument&) = nullptr;
		conRule cRule;
		std::wstring name;
		operationProperties properties;

		operation(const std::wstring& n, const conRule& cr,
			const operationProperties& prop,
			bool(*op)(const operationArgument&) = nullptr)
			:operationPtr(op), cRule(cr), name(n), properties(prop) {}
		operation() {}

		/*operation(const std::wstring& n, const conRuleInterface* con0,
			const conRuleInterface* con1, const conRuleInterface* con2,
			const operationProperties& prop,
			bool(*op)(const operationArgument&) = nullptr)
			:operation(n, conRule(con0, con1, con2), prop, op) {}*/
	};
}
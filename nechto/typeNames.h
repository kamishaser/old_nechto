
#pragma once

#include "baseValueTypes.h"
#include "mathOperator.h"

#include <string>
#include <vector>

namespace nechto
{
	namespace typeName
	{
		const ushort find(const std::vector<std::string>& NameSet, const std::string name)
		{
			for (int i = 0; i < NameSet.size(); i++)
				if (NameSet[i] == name)
					return i;
			return 0;

		}
		const std::vector<std::string> nodeT
		{
			"Error",				
			"Hub",					//разветвитель
			"Variable",				//объект-переменна€ базового типа, хнан€ща€с€ внутри алгоритма (одинаков дл€ всех исполнителей)
			"TypeCastOperator",		//оператор преобразовани€ типа данных
			"MathOperator",			//математический оператор
			"ConditionalBranching",	//if
			"ExternalFunction",				//функци€, не €вл€юща€с€ частью nechto
			"Tag",					//помечнный извне объект (односторонн€€ св€зь может быть только к метке 
									//(о метке знает только один объект))
		};
		
		const std::vector<std::string> variableT
		{
			"Error",
			"Int64",
			"Float",
			"Double",
		};
		
		const std::vector<std::string> mathOperatorT
		{
			"Error",
			"Assigment",		// =		
			"UnaryMinus",		// -

			"Addition",			// +
			"Subtraction",		// -

			"Multiplication",	// *
			"Division",			// /

			"Equal",			// ==
			"NotEqual",			// !=

			"Less",				// <
			"Greater",			// >
			"LessOrEqual",		// <=
			"GreaterOrEqual",	// >=

			"LogicNegation",	// !
			"LogicAnd",			// &&
			"LogicOr",			// ||
			"LogicExclusive",	// ^

			"Increment",		// ++
			"Decrement"			// --
		};

		ushort findSubType(ushort type, std::string subtypeName)
		{
			switch (type)
			{
			case node::Variable:
				return find(variableT, subtypeName);
			case node::MathOperator:
				return find(mathOperatorT, subtypeName);
			default:
				return 0;
			}
		}
	}
}
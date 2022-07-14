
#pragma once

#include "baseValueTypes.h"
#include "mathOperator.h"

#include <string>
#include <vector>

namespace nechto
{
	namespace typeName
	{
		const char find(const std::vector<std::wstring>& NameSet, const std::wstring name)
		{
			for (int i = 0; i < NameSet.size(); i++)
				if (NameSet[i] == name)
					return i;
			return 0;

		}
		const std::vector<std::wstring> nodeT
		{
			L"Error",				
			L"Hub",					//разветвитель
			L"Variable",				//объект-переменна€ базового типа, хнан€ща€с€ внутри алгоритма (одинаков дл€ всех исполнителей)
			L"TypeCastOperator",		//оператор преобразовани€ типа данных
			L"MathOperator",			//математический оператор
			L"ConditionalBranching",	//if
			L"ExternalFunction",		//функци€, не €вл€юща€с€ частью nechto
			L"Tag",					//помечнный извне объект (односторонн€€ св€зь может быть только к метке 
									//(о метке знает только один объект))
			L"Pointer"
		};
		
		const std::vector<std::wstring> variableT
		{
			L"Error",
			L"Int64",
			L"Double",
		};

		const std::vector<std::wstring> tagT
		{
			L"Error",
			L"Comment",
			L"Name",
			L"ExternalConnection",
			L"Attribute"
		};
		
		const std::vector<std::wstring> mathOperatorT
		{
			L"Error",
			L"Assigment",		// =		
			L"UnaryMinus",		// -

			L"Addition",			// +
			L"Subtraction",		// -

			L"Multiplication",	// *
			L"Division",			// /

			L"Equal",			// ==
			L"NotEqual",			// !=

			L"Less",				// <
			L"Greater",			// >
			L"LessOrEqual",		// <=
			L"GreaterOrEqual",	// >=

			L"LogicNegation",	// !
			L"LogicAnd",			// &&
			L"LogicOr",			// ||
			L"LogicExclusive",	// ^

			L"Increment",		// ++
			L"Decrement"			// --
		};

		char findSubtype(char type, std::wstring subtypeName)
		{
			switch (type)
			{
			case node::Variable:
				return find(variableT, subtypeName);
			case node::MathOperator:
				return find(mathOperatorT, subtypeName);
			case node::Tag:
				return find(tagT, subtypeName);
			default:
				return 0;
			}
		}
	}
}
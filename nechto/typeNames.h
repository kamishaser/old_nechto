
#pragma once

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
			L"Deleted",				
			L"Hub",					//разветвитель
			L"Variable",				//объект-переменная базового типа, хнанящаяся внутри алгоритма (одинаков для всех исполнителей)
			L"MathOperator",			//математический оператор
			L"ConditionalBranching",	//if
			L"ExternalFunction",		//функция, не являющаяся частью nechto
			L"Tag",					//строковый объект. Может использоваться как метка, название, комментарий или переменная
			L"Pointer",
			L"Array"
		};
		
		const std::vector<std::wstring> variableT
		{
			L"F64",
			L"I64",
		};

		const std::vector<std::wstring> tagT
		{
			L"Comment",
			L"Name",
			L"ExternalConnection",
			L"Attribute"
		};
		
		const std::vector<std::wstring> mathOperatorT
		{
			L"Assigment",			// =		
			L"UnaryMinus",			// -

			L"Addition+",			// +
			L"Subtraction",			// -

			L"Multiplication",		// *
			L"Division",			// /

			L"Equal",				// ==
			L"NotEqual",			// !=

			L"Less",				// <
			L"Greater",				// >
			L"LessOrEqual",			// <=
			L"GreaterOrEqual",		// >=

			L"LogicNegation",		// !
			L"LogicAnd",			// &&
			L"LogicOr",				// ||

			L"Increment",			// ++
			L"Decrement"			// --
		};

		const std::vector<std::wstring> mathOperatorShortT
		{
			L"=",
			L"0-",

			L"+",
			L"-",

			L"*",
			L"/",

			L"==",
			L"!=",

			L"<",
			L">",
			L"<=",
			L">=",

			L"!",
			L"&&",
			L"||",

			L"++",
			L"--"
		};


		char findSubtype(char type, std::wstring subtypeName)
		{
			switch (type)
			{
			case node::Variable:
				return find(variableT, subtypeName);
			case node::MathOperator:
				return find(mathOperatorT, subtypeName);
			case node::Text:
				return find(tagT, subtypeName);
			default:
				return 0;
			}
		}
	}
}
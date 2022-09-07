
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
			L"Variable",				//объект-переменна€ базового типа, хнан€ща€с€ внутри алгоритма (одинаков дл€ всех исполнителей)
			L"MathOperator",			//математический оператор
			L"ConditionalBranching",	//if
			L"Text",					//строковый объект. ћожет использоватьс€ как метка, название, комментарий или переменна€
			L"ExternalObject",
			L"Method",
			L"Pointer",
			L"Group"
		};
		
		const std::vector<std::wstring> variableT
		{
			L"F64",
			L"I64",
		};

		const std::vector<std::wstring> textT
		{
			L"Comment",
			L"Name",
			L"Other"
		};

		const std::vector<std::wstring> pointerT
		{
			L"Reference",//одиночна€ ссылка
			L"ConIter",//итератор соединений
			L"GroupIter"//итератор массива
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
				return find(textT, subtypeName);
			default:
				return 0;
			}
		}
	}
}

#pragma once

#include "mathOperator.h"

#include <string>
#include <vector>

namespace nechto
{
	namespace typeName
	{
		const char find(const std::vector<std::u16string>& NameSet, const std::u16string name)
		{
			for (int i = 0; i < NameSet.size(); i++)
				if (NameSet[i] == name)
					return i;
			return 0;

		}
		const std::vector<std::u16string> nodeT
		{
			u"Deleted",				
			u"Hub",					//������������
			u"Variable",				//������-���������� �������� ����, ���������� ������ ��������� (�������� ��� ���� ������������)
			u"MathOperator",			//�������������� ��������
			u"ConditionalBranching",	//if
			u"ExternalObject",
			u"Method",
			u"Pointer",
			u"Group",
			u"Text",					//��������� ������. ����� �������������� ��� �����, ��������, ����������� ��� ����������
			u"NodeOperator"
		};
		
		const std::vector<std::u16string> variableT
		{
			u"F64",
			u"I64",
		};

		const std::vector<std::u16string> pointerT
		{
			u"Reference",//��������� ������
			u"ConIter",//�������� ����������
			u"GroupIter"//�������� �������
		};
		
		
		const std::vector<std::u16string> mathOperatorT
		{
			u"Assigment",			// =		
			u"UnaryMinus",			// -

			u"Addition+",			// +
			u"Subtraction",			// -

			u"Multiplication",		// *
			u"Division",			// /

			u"Equau",				// ==
			u"NotEquau",			// !=

			u"Less",				// <
			u"Greater",				// >
			u"LessOrEquau",			// <=
			u"GreaterOrEquau",		// >=

			u"LogicNegation",		// !
			u"LogicAnd",			// &&
			u"LogicOr",				// ||

			u"Increment",			// ++
			u"Decrement"			// --
		};

		const std::vector<std::u16string> mathOperatorShortT
		{
			u"=",
			u"0-",

			u"+",
			u"-",

			u"*",
			u"/",

			u"==",
			u"!=",

			u"<",
			u">",
			u"<=",
			u">=",

			u"!",
			u"&&",
			u"||",

			u"++",
			u"--"
		};


		char findSubtype(char type, std::u16string subtypeName)
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
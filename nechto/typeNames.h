#pragma once
#include "typeDeclarations.h"
#include <functional>

namespace nechto::typeName
{
	const std::wstring getTypeName(char type)
	{
		switch (type)
		{
		case nechto::nodeT::Deleted:
			return L"Deleted";
		case nechto::nodeT::Hub:
			return L"Hub";
		case nechto::nodeT::Group:
			return L"Group";
		case nechto::nodeT::Pointer:
			return L"Pointer";
		case nechto::nodeT::Variable:
			return L"Variable";
		case nechto::nodeT::Object:
			return L"Object";
		case nechto::nodeT::Text:
			return L"Text";
		case nechto::nodeT::MathOperator:
			return L"MathOperator";
		case nechto::nodeT::Condition:
			return L"Condition";
		case nechto::nodeT::Method:
			return L"Method";
		case nechto::nodeT::NodeOperator:
			return L"NodeOperator";
		default:
			return L"Error";
			//return L"";
		}
	}
	std::wstring getGroupSubtypeName(char subtype)
	{
		switch (subtype)
		{
		case nechto::groupT::weak:
			return L"weak";
		case nechto::groupT::strong:
			return L"strong";
		default:
			return L"error";
		}		
	}
	std::wstring getPointerSubtypeName(char subtype)
	{
		switch (subtype)
		{
		case nechto::pointerT::Simple:
			return L"Simple";
		case nechto::pointerT::PortIter:
			return L"PortIter";
		case nechto::pointerT::GroupIter:
			return L"GroupIter";
		default:
			return L"Error";
			//return L"";
		}
	}
	std::wstring getVariableSubtypeName(char subtype)
	{
		switch (subtype)
		{
		case nechto::variableT::F64:
			return L"F64";
		case nechto::variableT::I64:
			return L"I64";
		default:
			return L"Error";
			//return L"";
		}
	}
	std::wstring getObjectSubtypeName(char subtype)
	{
		switch (subtype)
		{
		case nechto::objectT::NotOwning:
			return L"NotOwning";
		case nechto::objectT::Owning:
			return L"Owning";
		default:
			return L"Error";
			//return L"";
		}
	}
	std::wstring getTextSubtypeName(char subtype)
	{
		switch (subtype)
		{
		case nechto::textT::NotOwning:
			return L"NotOwning";
		case nechto::textT::Owning:
			return L"Owning";
		default:
			return L"Error";
			//return L"";
		}
	}
	std::wstring getMathOperatorSubtypeName(char subtype)
	{
		switch (subtype)
		{
		case nechto::mathOperatorT::Assigment:
			return L"Assigment";
		case nechto::mathOperatorT::UnaryMinus:
			return L"UnaryMinus";
		case nechto::mathOperatorT::Addition:
			return L"Addition";
		case nechto::mathOperatorT::Subtraction:
			return L"Subtraction";
		case nechto::mathOperatorT::Multiplication:
			return L"Multiplication";
		case nechto::mathOperatorT::Division:
			return L"Division";
		case nechto::mathOperatorT::Equal:
			return L"Equal";
		case nechto::mathOperatorT::NotEqual:
			return L"NotEqual";
		case nechto::mathOperatorT::Less:
			return L"Less";
		case nechto::mathOperatorT::Greater:
			return L"Greater";
		case nechto::mathOperatorT::LessOrEqual:
			return L"LessOrEqual";
		case nechto::mathOperatorT::GreaterOrEqual:
			return L"GreaterOrEqual";
		case nechto::mathOperatorT::LogicNegation:
			return L"LogicNegation";
		case nechto::mathOperatorT::LogicAnd:
			return L"LogicAnd";
		case nechto::mathOperatorT::LogicOr:
			return L"LogicOr";
		case nechto::mathOperatorT::Increment:
			return L"Increment";
		case nechto::mathOperatorT::Decrement:
			return L"Decrement";
		default:
			return L"Error";
			//return L"";
		}
	}
	std::wstring getMathOperatorShortSubtypeName(char subtype)
	{
		switch (subtype)
		{
		case nechto::mathOperatorT::Assigment:
			return L"=";
		case nechto::mathOperatorT::UnaryMinus:
			return L"-";
		case nechto::mathOperatorT::Addition:
			return L"+";
		case nechto::mathOperatorT::Subtraction:
			return L"-";
		case nechto::mathOperatorT::Multiplication:
			return L"*";
		case nechto::mathOperatorT::Division:
			return L"/";
		case nechto::mathOperatorT::Equal:
			return L"==";
		case nechto::mathOperatorT::NotEqual:
			return L"!=";
		case nechto::mathOperatorT::Less:
			return L"<";
		case nechto::mathOperatorT::Greater:
			return L">";
		case nechto::mathOperatorT::LessOrEqual:
			return L"<=";
		case nechto::mathOperatorT::GreaterOrEqual:
			return L">=";
		case nechto::mathOperatorT::LogicNegation:
			return L"!";
		case nechto::mathOperatorT::LogicAnd:
			return L"&&";
		case nechto::mathOperatorT::LogicOr:
			return L"||";
		case nechto::mathOperatorT::Increment:
			return L"++";
		case nechto::mathOperatorT::Decrement:
			return L"--";
		default:
			return L"Err";
			//return L"";
		}
	}
	std::wstring getSubtypeName(char type, char subtype)
	{
		switch (type)
		{
		case nechto::nodeT::Deleted:
			return L"";
		case nechto::nodeT::Hub:
			return L"";
		case nechto::nodeT::Group:
			return getGroupSubtypeName(subtype);
		case nechto::nodeT::Pointer:
			return getPointerSubtypeName(subtype);
		case nechto::nodeT::Variable:
			return getVariableSubtypeName(subtype);
		case nechto::nodeT::Object:
			return getObjectSubtypeName(subtype);
		case nechto::nodeT::Text:
			return getTextSubtypeName(subtype);
		case nechto::nodeT::MathOperator:
			return getMathOperatorSubtypeName(subtype);
		case nechto::nodeT::Condition:
			return L"";
		case nechto::nodeT::Method:
			return L"";
		case nechto::nodeT::NodeOperator:
			return L"";
		default:
			return L"???";
		}
	}
	std::wstring getTypeAndSubtypeName(char type, char subtype, std::wstring separator = L" ")
	{
		return getTypeName(type) + separator + getSubtypeName(type, subtype);
	}
	std::vector<std::wstring> getTypeNameList(const std::function<std::wstring(char)>& func, char max)
	{
		std::vector<std::wstring> list;
		for (char i = 0; i < max; ++i)
			list.push_back(func(i));
		return list;
	}
}
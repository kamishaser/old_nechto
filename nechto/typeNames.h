#pragma once
#include "typeDeclarations.h"
#include <functional>

namespace nechto::typeName
{
	const std::wstring getTypeName(unsigned char type)
	{
		switch (type)
		{
		case nechto::nodeT::Deleted:
			return L"Deleted";
		case nechto::nodeT::Hub:
			return L"Hub";
		case nechto::nodeT::Group:
			return L"Group";
		case nechto::nodeT::Iterator:
			return L"Iterator";
		case nechto::nodeT::Variable:
			return L"Variable";
		case nechto::nodeT::Object:
			return L"Object";
		case nechto::nodeT::Text:
			return L"Text";
		case nechto::nodeT::MathOperation:
			return L"MathOperation";
		case nechto::nodeT::Condition:
			return L"Condition";
		case nechto::nodeT::Method:
			return L"Method";
		default:
			return L"Error";
			//return L"";
		}
	}
	std::wstring getGroupSubtypeName(unsigned char subtype)
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
	std::wstring getIteratorSubtypeName(unsigned char subtype)
	{
		switch (subtype)
		{
		case nechto::iteratorT::PortIter:
			return L"PortIter";
		case nechto::iteratorT::GroupIter:
			return L"GroupIter";
		default:
			return L"Error";
			//return L"";
		}
	}
	std::wstring getVariableSubtypeName(unsigned char subtype)
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
	std::wstring getObjectSubtypeName(unsigned char subtype)
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
	std::wstring getTextSubtypeName(unsigned char subtype)
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
	std::wstring getMathOperationSubtypeName(unsigned char subtype)
	{
		switch (subtype)
		{
		case nechto::MathOperationT::Assigment:
			return L"Assigment";
		case nechto::MathOperationT::UnaryMinus:
			return L"UnaryMinus";
		case nechto::MathOperationT::Addition:
			return L"Addition";
		case nechto::MathOperationT::Subtraction:
			return L"Subtraction";
		case nechto::MathOperationT::Multiplication:
			return L"Multiplication";
		case nechto::MathOperationT::Division:
			return L"Division";
		case nechto::MathOperationT::Equal:
			return L"Equal";
		case nechto::MathOperationT::NotEqual:
			return L"NotEqual";
		case nechto::MathOperationT::Less:
			return L"Less";
		case nechto::MathOperationT::Greater:
			return L"Greater";
		case nechto::MathOperationT::LessOrEqual:
			return L"LessOrEqual";
		case nechto::MathOperationT::GreaterOrEqual:
			return L"GreaterOrEqual";
		case nechto::MathOperationT::LogicNegation:
			return L"LogicNegation";
		case nechto::MathOperationT::LogicAnd:
			return L"LogicAnd";
		case nechto::MathOperationT::LogicOr:
			return L"LogicOr";
		case nechto::MathOperationT::Increment:
			return L"Increment";
		case nechto::MathOperationT::Decrement:
			return L"Decrement";
		default:
			return L"Error";
			//return L"";
		}
	}
	std::wstring getMathOperationShortSubtypeName(unsigned char subtype)
	{
		switch (subtype)
		{
		case nechto::MathOperationT::Assigment:
			return L"=";
		case nechto::MathOperationT::UnaryMinus:
			return L"-";
		case nechto::MathOperationT::Addition:
			return L"+";
		case nechto::MathOperationT::Subtraction:
			return L"-";
		case nechto::MathOperationT::Multiplication:
			return L"*";
		case nechto::MathOperationT::Division:
			return L"/";
		case nechto::MathOperationT::Equal:
			return L"==";
		case nechto::MathOperationT::NotEqual:
			return L"!=";
		case nechto::MathOperationT::Less:
			return L"<";
		case nechto::MathOperationT::Greater:
			return L">";
		case nechto::MathOperationT::LessOrEqual:
			return L"<=";
		case nechto::MathOperationT::GreaterOrEqual:
			return L">=";
		case nechto::MathOperationT::LogicNegation:
			return L"!";
		case nechto::MathOperationT::LogicAnd:
			return L"&&";
		case nechto::MathOperationT::LogicOr:
			return L"||";
		case nechto::MathOperationT::Increment:
			return L"++";
		case nechto::MathOperationT::Decrement:
			return L"--";
		default:
			return L"Err";
			//return L"";
		}
	}
	std::wstring getSubtypeName(unsigned char type, unsigned char subtype)
	{
		switch (type)
		{
		case nechto::nodeT::Deleted:
			return L"";
		case nechto::nodeT::Hub:
			return L"";
		case nechto::nodeT::Group:
			return getGroupSubtypeName(subtype);
		case nechto::nodeT::Iterator:
			return getIteratorSubtypeName(subtype);
		case nechto::nodeT::Variable:
			return getVariableSubtypeName(subtype);
		case nechto::nodeT::Object:
			return getObjectSubtypeName(subtype);
		case nechto::nodeT::Text:
			return getTextSubtypeName(subtype);
		case nechto::nodeT::MathOperation:
			return getMathOperationSubtypeName(subtype);
		case nechto::nodeT::Condition:
			return L"";
		case nechto::nodeT::Method:
			return L"";
		default:
			return L"???";
		}
	}
	std::wstring getTypeAndSubtypeName(unsigned char type, unsigned char subtype, std::wstring separator = L" ")
	{
		return getTypeName(type) + separator + getSubtypeName(type, subtype);
	}
	std::vector<std::wstring> getTypeNameList(const std::function<std::wstring(char)>& func, unsigned char max)
	{
		std::vector<std::wstring> list;
		for (char i = 0; i < max; ++i)
			list.push_back(func(i));
		return list;
	}
}
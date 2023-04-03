#pragma once
#include "typeDeclarations.h"
#include <functional>
#include <string>

namespace nechto::typeName
{
	constexpr const std::wstring getTypeName(unsigned char type)
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
		case nechto::nodeT::Entity:
			return L"Entity";
		case nechto::nodeT::Text:
			return L"Text";
		case nechto::nodeT::MathOperation:
			return L"MathOperation";
		case nechto::nodeT::Condition:
			return L"Condition";
		case nechto::nodeT::Path:
			return L"Path";
		default:
			return L"Error";
			//return L"";
		}
	}
	constexpr const std::wstring getGroupSubtypeName(unsigned char subtype)
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
	constexpr const std::wstring getPointerSubtypeName(unsigned char subtype)
	{
		switch (subtype)
		{
		case nechto::pointerT::PortPointer:
			return L"PortPointer";
		case nechto::pointerT::GroupPointer:
			return L"GroupPointer";
		default:
			return L"Error";
			//return L"";
		}
	}
	constexpr const std::wstring getVariableSubtypeName(unsigned char subtype)
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
	constexpr const std::wstring getEntitySubtypeName(unsigned char subtype)
	{
		switch (subtype)
		{
		case nechto::entityT::oneSideLink:
			return L"OneSideLink";
		case nechto::entityT::singleConnection:
			return L"SingleConnection";
		case nechto::entityT::multipleConnection:
			return L"MultipleConnection";
		default:
			return L"Error";
		}
	}
	constexpr const std::wstring getMathOperationSubtypeName(unsigned char subtype)
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
	constexpr const std::wstring getMathOperationShortSubtypeName(unsigned char subtype)
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
	constexpr const std::wstring getVectorSubtypeName(unsigned char subtype)
	{
		//VectorT::Type type = subtype;
		return L"error";
	}
	constexpr const std::wstring getOperatorSubtypeName(unsigned char subtype)
	{
		//OperatorT::Type type = subtype;
		return L"error";
	}
	constexpr const std::wstring getVectorOperationSubtypeName(unsigned char subtype)
	{
		//VectorOperationT::Type type = subtype;
		return L"error";
	}
	constexpr const std::wstring getConnectionOperationSubtypeName(unsigned char subtype)
	{
		//ConnectionOperationT::Type type = subtype;
		return L"error";
	}
	constexpr const std::wstring getCreationOperationSubtypeName(unsigned char subtype)
	{
		//CreationOperationT::Type type = subtype;
		return L"error";
	}
	constexpr const std::wstring getPointerationOperationSubtypeName(unsigned char subtype)
	{
		//PointerationOperationT::Type type = subtype;
		return L"error";
	}
	constexpr const std::wstring getHubManagementSubtypeName(unsigned char subtype)
	{
		//HubManagementT::Type type = subtype;
		return L"error";
	}
	constexpr const std::wstring getOperatorManagementSubtypeName(unsigned char subtype)
	{
		//OperatorManagementT::Type type = subtype;
		return L"error";
	}
	constexpr const std::wstring getReadingOperationSubtypeName(unsigned char subtype)
	{
		//ReadingOperationT::Type type = subtype;
		return L"error";
	}
	constexpr const std::wstring getTextOperationSubtypeName(unsigned char subtype)
	{
		//TextOperationT::Type type = subtype;
		return L"error";
	}
	constexpr const std::wstring getStructSubtypeName(unsigned char subtype)
	{
		//TextOperationT::Type type = subtype;
		return L"error";
	}
	constexpr const std::wstring getSubtypeName(unsigned char type, unsigned char subtype)
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
		case nechto::nodeT::Entity:
			return getEntitySubtypeName(subtype);
		case nechto::nodeT::Text:
			return getEntitySubtypeName(subtype);
		case nechto::nodeT::MathOperation:
			return getMathOperationSubtypeName(subtype);
		case nechto::nodeT::Condition:
			return L"";
		case nechto::nodeT::Path:
			return L"";
		case nodeT::Vector:
			return getVectorSubtypeName(subtype);
		case nodeT::VectorOperation:
			return getVectorOperationSubtypeName(subtype);
		case nodeT::ConnectionOperation:
			return getConnectionOperationSubtypeName(subtype);
		case nodeT::CreationOperation:
			return getCreationOperationSubtypeName(subtype);
		case nodeT::PointerationOperation:
			return getPointerationOperationSubtypeName(subtype);
		case nodeT::ReadingOperation:
			return getReadingOperationSubtypeName(subtype);
		case nodeT::TextOperation:
			return getTextOperationSubtypeName(subtype);
		case nodeT::HubManagement:
			return getHubManagementSubtypeName(subtype);
		case nodeT::OperatorManagement:
			return getOperatorManagementSubtypeName(subtype);
		case nodeT::Operator:
			return getEntitySubtypeName(subtype);
		case nodeT::Struct:
			return getStructSubtypeName(subtype);
		default:
			return L"???";
		}
	}
	constexpr const std::wstring getTypeAndSubtypeName(unsigned char type, unsigned char subtype, std::wstring separator = L" ")
	{
		return getTypeName(type) + separator + getSubtypeName(type, subtype);
	}
	constexpr const std::vector<std::wstring> getTypeNameList(const std::function<std::wstring(char)>& func, unsigned char max)
	{
		std::vector<std::wstring> list;
		for (char i = 0; i < max; ++i)
			list.push_back(func(i));
		return list;
	}
}
#pragma once
#include "typeDeclarations.h"

namespace nechto::typeName
{
	const ustr getTypeName(char type)
	{
		switch (type)
		{
		case nechto::nodeT::Deleted:
			return u"Deleted";
		case nechto::nodeT::Hub:
			return u"Hub";
		case nechto::nodeT::Group:
			return u"Group";
		case nechto::nodeT::Pointer:
			return u"Pointer";
		case nechto::nodeT::Variable:
			return u"Variable";
		case nechto::nodeT::Object:
			return u"Object";
		case nechto::nodeT::Text:
			return u"Text";
		case nechto::nodeT::MathOperator:
			return u"MathOperator";
		case nechto::nodeT::Condition:
			return u"Condition";
		case nechto::nodeT::Method:
			return u"Method";
		case nechto::nodeT::NodeOperator:
			return u"NodeOperator";
		default:
			return u"Error";
			//return u"";
		}
	}
	ustr getGroupSubtypeName(char subtype)
	{
		switch (subtype)
		{
		case nechto::groupT::weak:
			return u"weak";
		case nechto::groupT::strong:
			return u"strong";
		default:
			return u"error";
		}		
	}
	ustr getPointerSubtypeName(char subtype)
	{
		switch (subtype)
		{
		case nechto::pointerT::Simple:
			return u"Simple";
		case nechto::pointerT::ConIter:
			return u"ConIter";
		case nechto::pointerT::GroupIter:
			return u"GroupIter";
		default:
			return u"Error";
			//return u"";
		}
	}
	ustr getVariableSubtypeName(char subtype)
	{
		switch (subtype)
		{
		case nechto::variableT::F64:
			return u"F64";
		case nechto::variableT::I64:
			return u"I64";
		default:
			return u"Error";
			//return u"";
		}
	}
	ustr getObjectSubtypeName(char subtype)
	{
		switch (subtype)
		{
		case nechto::objectT::NotOwning:
			return u"NotOwning";
		case nechto::objectT::Owning:
			return u"Owning";
		default:
			return u"Error";
			//return u"";
		}
	}
	ustr getTextSubtypeName(char subtype)
	{
		switch (subtype)
		{
		case nechto::textT::NotOwning:
			return u"NotOwning";
		case nechto::textT::Owning:
			return u"Owning";
		default:
			return u"Error";
			//return u"";
		}
	}
	ustr getMathOperatorSubtypeName(char subtype)
	{
		switch (subtype)
		{
		case nechto::mathOperatorT::Assigment:
			return u"Assigment";
		case nechto::mathOperatorT::UnaryMinus:
			return u"UnaryMinus";
		case nechto::mathOperatorT::Addition:
			return u"Addition";
		case nechto::mathOperatorT::Subtraction:
			return u"Subtraction";
		case nechto::mathOperatorT::Multiplication:
			return u"Multiplication";
		case nechto::mathOperatorT::Division:
			return u"Division";
		case nechto::mathOperatorT::Equal:
			return u"Equal";
		case nechto::mathOperatorT::NotEqual:
			return u"NotEqual";
		case nechto::mathOperatorT::Less:
			return u"Less";
		case nechto::mathOperatorT::Greater:
			return u"Greater";
		case nechto::mathOperatorT::LessOrEqual:
			return u"LessOrEqual";
		case nechto::mathOperatorT::GreaterOrEqual:
			return u"GreaterOrEqual";
		case nechto::mathOperatorT::LogicNegation:
			return u"LogicNegation";
		case nechto::mathOperatorT::LogicAnd:
			return u"LogicAnd";
		case nechto::mathOperatorT::LogicOr:
			return u"LogicOr";
		case nechto::mathOperatorT::Increment:
			return u"Increment";
		case nechto::mathOperatorT::Decrement:
			return u"Decrement";
		default:
			return u"Error";
			//return u"";
		}
	}
	ustr getMathOperatorShortSubtypeName(char subtype)
	{
		switch (subtype)
		{
		case nechto::mathOperatorT::Assigment:
			return u"=";
		case nechto::mathOperatorT::UnaryMinus:
			return u"-";
		case nechto::mathOperatorT::Addition:
			return u"+";
		case nechto::mathOperatorT::Subtraction:
			return u"-";
		case nechto::mathOperatorT::Multiplication:
			return u"*";
		case nechto::mathOperatorT::Division:
			return u"/";
		case nechto::mathOperatorT::Equal:
			return u"==";
		case nechto::mathOperatorT::NotEqual:
			return u"!=";
		case nechto::mathOperatorT::Less:
			return u"<";
		case nechto::mathOperatorT::Greater:
			return u">";
		case nechto::mathOperatorT::LessOrEqual:
			return u"<=";
		case nechto::mathOperatorT::GreaterOrEqual:
			return u">=";
		case nechto::mathOperatorT::LogicNegation:
			return u"!";
		case nechto::mathOperatorT::LogicAnd:
			return u"&&";
		case nechto::mathOperatorT::LogicOr:
			return u"||";
		case nechto::mathOperatorT::Increment:
			return u"++";
		case nechto::mathOperatorT::Decrement:
			return u"--";
		default:
			return u"Err";
			//return u"";
		}
	}
	ustr getSubtypeName(char type, char subtype)
	{
		switch (type)
		{
		case nechto::nodeT::Deleted:
			return u"";
		case nechto::nodeT::Hub:
			return u"";
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
			return u"";
		case nechto::nodeT::Method:
			return u"";
		case nechto::nodeT::NodeOperator:
			return u"";
		default:
			return u"???";
		}
	}
	ustr getTypeAndSubtypeName(char type, char subtype, ustr separator = u" ")
	{
		return getTypeName(type) + separator + getSubtypeName(type, subtype);
	}
}
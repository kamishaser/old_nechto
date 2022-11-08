#pragma once
#include "variablePtr.h"
#include "nodeOperationSet.h"

namespace nechto
{
	namespace mathOperationF
	{
		bool assigmentF(
			variablePtr result, const variablePtr fArg)
		{
			if (result.subtype() && fArg.subtype())
				result = (i64)fArg;
			else
				result = (f64)fArg;
			return true;
		}
		bool unaryMinusF(
			variablePtr result, const variablePtr fArg)
		{
			if (result.subtype() && fArg.subtype())
				result = -(i64)fArg;
			else
				result = -(f64)fArg;
			return true;
		}
		bool additionF(
			variablePtr result, const variablePtr fArg, const variablePtr sArg)
		{
			if (result.subtype() && fArg.subtype() && sArg.subtype())
				result = (i64)fArg + (i64)sArg;
			else
				result = (f64)fArg + (f64)sArg;
			return true;
		}
		bool subtractionF(
			variablePtr result, const variablePtr fArg, const variablePtr sArg)
		{
			if (result.subtype() && fArg.subtype() && sArg.subtype())
				result = (i64)fArg - (i64)sArg;
			else
				result = (f64)fArg - (f64)sArg;
			return true;
		}
		bool multiplicationF(
			variablePtr result, const variablePtr fArg, const variablePtr sArg)
		{
			if (result.subtype() && fArg.subtype() && sArg.subtype())
				result = (i64)fArg * (i64)sArg;
			else
				result = (f64)fArg * (f64)sArg;
			return true;
		}
		bool divisionF(
			variablePtr result, const variablePtr fArg, const variablePtr sArg)
		{
			if (result.subtype() && fArg.subtype() && sArg.subtype())
				result = (i64)fArg - (i64)sArg;
			else
				result = (f64)fArg - (f64)sArg;
			return true;
		}
		bool equalF(
			i64VariablePtr result, const variablePtr fArg, const variablePtr sArg)
		{
			if (fArg.subtype() && sArg.subtype())
				result = (i64)fArg == (i64)sArg;
			else
				result = (f64)fArg == (f64)sArg;
			return true;
		}
		bool notEqualF(
			i64VariablePtr result, const variablePtr fArg, const variablePtr sArg)
		{
			if (fArg.subtype() && sArg.subtype())
				result = (i64)fArg != (i64)sArg;
			else
				result = (f64)fArg != (f64)sArg;
			return true;
		}
		bool lessF(
			i64VariablePtr result, const variablePtr fArg, const variablePtr sArg)
		{
			if (fArg.subtype() && sArg.subtype())
				result = (i64)fArg == (i64)sArg;
			else
				result = (f64)fArg == (f64)sArg;
			return true;
		}
		bool greaterF(
			i64VariablePtr result, const variablePtr fArg, const variablePtr sArg)
		{
			if (fArg.subtype() && sArg.subtype())
				result = (i64)fArg != (i64)sArg;
			else
				result = (f64)fArg != (f64)sArg;
			return true;
		}
		bool lessOrEqualF(
			i64VariablePtr result, const variablePtr fArg, const variablePtr sArg)
		{
			if (fArg.subtype() && sArg.subtype())
				result = (i64)fArg == (i64)sArg;
			else
				result = (f64)fArg == (f64)sArg;
			return true;
		}
		bool greaterOrEqualF(
			i64VariablePtr result, const variablePtr fArg, const variablePtr sArg)
		{
			if (fArg.subtype() && sArg.subtype())
				result = (i64)fArg != (i64)sArg;
			else
				result = (f64)fArg != (f64)sArg;
			return true;
		}
		bool logicNegationF(
			i64VariablePtr result, const i64VariablePtr fArg)
		{
			result = !fArg;
			return true;
		}
		bool logicAndF(
			i64VariablePtr result, const i64VariablePtr fArg, const i64VariablePtr sArg)
		{
			result = fArg && sArg;
			return true;
		}
		bool logicOrF(
			i64VariablePtr result, const i64VariablePtr fArg, const i64VariablePtr sArg)
		{
			result = fArg || sArg;
			return true;
		}
		bool incrementF(
			i64VariablePtr Arg)
		{
			Arg = Arg + 1;
			return true;
		}
		bool decrementF(
			i64VariablePtr Arg)
		{
			Arg = Arg - 1;
			return true;
		}
	}
	class mathPtr : public existing<nodePtr>
	{
	public:
		static const staticNodeOperationSet operSet;
		mathPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::Math;
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
	};
	const staticNodeOperationSet mathPtr::operSet
	{
		{L"assigment", {mathOperationF::assigmentF}},
		{L"unaryMinus", {mathOperationF::unaryMinusF}},

		{L"addition", {mathOperationF::additionF}},
		{L"subtraction", {mathOperationF::subtractionF}},

		{L"multiplication", {mathOperationF::multiplicationF}},
		{L"division", {mathOperationF::divisionF}},

		{L"equal", {mathOperationF::equalF}},
		{L"notEqual", {mathOperationF::notEqualF}},

		{L"less", {mathOperationF::lessF}},
		{L"greater", {mathOperationF::greaterF}},
		{L"lessOrEqual", {mathOperationF::lessOrEqualF}},
		{L"greaterOrEqual", {mathOperationF::greaterOrEqualF}},

		{L"logicNegation", {mathOperationF::logicNegationF}},
		{L"logicAnd", {mathOperationF::logicAndF}},
		{L"logicOr", {mathOperationF::logicOrF}},

		{L"increment", {mathOperationF::incrementF}},
		{L"decrement", {mathOperationF::decrementF}}
	};
}
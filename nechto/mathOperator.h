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
	class mathOperatorPtr : public existing<nodePtr>
	{
	public:
		static const staticNodeOperationSet operSet;
		mathOperatorPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::MathOperator;
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
		const operation* getOperation() const
		{
			return &operSet.getOperation(subtype());
		}
	};
	const staticNodeOperationSet mathOperatorPtr::operSet
	{
		{u"assigment", {mathOperationF::assigmentF}},
		{u"unaryMinus", {mathOperationF::unaryMinusF}},

		{u"addition", {mathOperationF::additionF}},
		{u"subtraction", {mathOperationF::subtractionF}},

		{u"multiplication", {mathOperationF::multiplicationF}},
		{u"division", {mathOperationF::divisionF}},

		{u"equau", {mathOperationF::equalF}},
		{u"notEquau", {mathOperationF::notEqualF}},

		{u"less", {mathOperationF::lessF}},
		{u"greater", {mathOperationF::greaterF}},
		{u"lessOrEquau", {mathOperationF::lessOrEqualF}},
		{u"greaterOrEquau", {mathOperationF::greaterOrEqualF}},

		{u"logicNegation", {mathOperationF::logicNegationF}},
		{u"logicAnd", {mathOperationF::logicAndF}},
		{u"logicOr", {mathOperationF::logicOrF}},

		{u"increment", {mathOperationF::incrementF}},
		{u"decrement", {mathOperationF::decrementF}}
	};
}
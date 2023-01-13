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
			result = !(fArg);
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
			Arg = Arg.get() + 1;
			return true;
		}
		bool decrementF(
			i64VariablePtr Arg)
		{
			Arg = Arg.get() - 1;
			return true;
		}
	}
	class MathOperationPtr : public existing<nodePtr>
	{
	public:
		static const staticNodeOperationSet operSet;
		MathOperationPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::MathOperation;
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
	const staticNodeOperationSet MathOperationPtr::operSet
	{
		{L"assigment", operation::wrap<ntV, const ntV, mathOperationF::assigmentF>()},
		{L"unaryMinus", operation::wrap<ntV, const ntV, mathOperationF::unaryMinusF>()},

		{L"addition", operation::wrap<ntV, const ntV, const ntV, mathOperationF::additionF>()},
		{L"subtraction", operation::wrap<ntV, const ntV, const ntV, mathOperationF::subtractionF>()},

		{L"multiplication", operation::wrap<ntV, const ntV, const ntV, mathOperationF::multiplicationF>()},
		{L"division", operation::wrap<ntV, const ntV, const ntV, mathOperationF::divisionF>()},

		{L"equal", operation::wrap<ntI64, const ntV, const ntV, mathOperationF::equalF>()},
		{L"notEqual", operation::wrap<ntI64, const ntV, const ntV, mathOperationF::notEqualF>()},

		{L"less", operation::wrap<ntI64, const ntV, const ntV, mathOperationF::lessF>()},
		{L"greater", operation::wrap<ntI64, const ntV, const ntV, mathOperationF::greaterF>()},
		{L"lessOrEqual", operation::wrap<ntI64, const ntV, const ntV, mathOperationF::lessOrEqualF>()},
		{L"greaterOrEqual", operation::wrap<ntI64, const ntV, const ntV, mathOperationF::greaterOrEqualF>()},

		{L"logicNegation", operation::wrap<ntI64, const ntI64, mathOperationF::logicNegationF>()},
		{L"logicAnd", operation::wrap<ntI64, const ntI64, const ntI64, mathOperationF::logicAndF>()},
		{L"logicOr", operation::wrap<ntI64, const ntI64, const ntI64, mathOperationF::logicOrF>()},

		{L"increment", operation::wrap<ntI64, mathOperationF::incrementF>()},
		{L"decrement", operation::wrap<ntI64, mathOperationF::decrementF>()}
	};
}
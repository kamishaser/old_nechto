#pragma once
#include "variablePtr.h"
#include "nodeOperationSet.h"
#include "crSet.h"
#include "typeNames.h"

namespace nechto
{
	namespace mathOperationF
	{
		bool assigmentF(
			const operationArgument& arg)
		{
			variablePtr result = arg.port0;
			variablePtr fArg = arg.port1;
			if (result.subtype() && fArg.subtype())
				result = (i64)fArg;
			else
				result = (f64)fArg;
			return true;
		}
		bool unaryMinusF(
			const operationArgument& arg)
		{
			variablePtr result = arg.port0;
			variablePtr fArg = arg.port1;
			if (result.subtype() && fArg.subtype())
				result = -(i64)fArg;
			else
				result = -(f64)fArg;
			return true;
		}
		bool additionF(
			const operationArgument& arg)
		{
			variablePtr result = arg.port0;
			variablePtr fArg = arg.port1;
			variablePtr sArg = arg.port2;
			if (result.subtype() && fArg.subtype() && sArg.subtype())
				result = (i64)fArg + (i64)sArg;
			else
				result = (f64)fArg + (f64)sArg;
			return true;
		}
		bool subtractionF(
			const operationArgument& arg)
		{
			variablePtr result = arg.port0;
			variablePtr fArg = arg.port1;
			variablePtr sArg = arg.port2;
			if (result.subtype() && fArg.subtype() && sArg.subtype())
				result = (i64)fArg - (i64)sArg;
			else
				result = (f64)fArg - (f64)sArg;
			return true;
		}
		bool multiplicationF(
			const operationArgument& arg)
		{
			variablePtr result = arg.port0;
			variablePtr fArg = arg.port1;
			variablePtr sArg = arg.port2;
			if (result.subtype() && fArg.subtype() && sArg.subtype())
				result = (i64)fArg * (i64)sArg;
			else
				result = (f64)fArg * (f64)sArg;
			return true;
		}
		bool divisionF(
			const operationArgument& arg)
		{
			variablePtr result = arg.port0;
			variablePtr fArg = arg.port1;
			variablePtr sArg = arg.port2;
			if (result.subtype() && fArg.subtype() && sArg.subtype())
				result = (i64)fArg - (i64)sArg;
			else
				result = (f64)fArg - (f64)sArg;
			return true;
		}
		bool equalF(
			const operationArgument& arg)
		{
			i64VariablePtr result = arg.port0;
			variablePtr fArg = arg.port1;
			variablePtr sArg = arg.port2;
			if (fArg.subtype() && sArg.subtype())
				result = (i64)fArg == (i64)sArg;
			else
				result = (f64)fArg == (f64)sArg;
			return true;
		}
		bool notEqualF(
			const operationArgument& arg)
		{
			i64VariablePtr result = arg.port0;
			variablePtr fArg = arg.port1;
			variablePtr sArg = arg.port2;
			if (fArg.subtype() && sArg.subtype())
				result = (i64)fArg != (i64)sArg;
			else
				result = (f64)fArg != (f64)sArg;
			return true;
		}
		bool lessF(
			const operationArgument& arg)
		{
			i64VariablePtr result = arg.port0;
			variablePtr fArg = arg.port1;
			variablePtr sArg = arg.port2;
			if (fArg.subtype() && sArg.subtype())
				result = (i64)fArg == (i64)sArg;
			else
				result = (f64)fArg == (f64)sArg;
			return true;
		}
		bool greaterF(
			const operationArgument& arg)
		{
			i64VariablePtr result = arg.port0;
			variablePtr fArg = arg.port1;
			variablePtr sArg = arg.port2;
			if (fArg.subtype() && sArg.subtype())
				result = (i64)fArg != (i64)sArg;
			else
				result = (f64)fArg != (f64)sArg;
			return true;
		}
		bool lessOrEqualF(
			const operationArgument& arg)
		{
			i64VariablePtr result = arg.port0;
			variablePtr fArg = arg.port1;
			variablePtr sArg = arg.port2;
			if (fArg.subtype() && sArg.subtype())
				result = (i64)fArg == (i64)sArg;
			else
				result = (f64)fArg == (f64)sArg;
			return true;
		}
		bool greaterOrEqualF(
			const operationArgument& arg)
		{
			i64VariablePtr result = arg.port0;
			variablePtr fArg = arg.port1;
			variablePtr sArg = arg.port2;
			if (fArg.subtype() && sArg.subtype())
				result = (i64)fArg != (i64)sArg;
			else
				result = (f64)fArg != (f64)sArg;
			return true;
		}
		bool logicNegationF(
			const operationArgument& arg)
		{
			i64VariablePtr result = arg.port0;
			i64VariablePtr fArg = arg.port1;
			result = !(fArg);
			return true;
		}
		bool logicAndF(
			const operationArgument& arg)
		{
			i64VariablePtr result = arg.port0;
			i64VariablePtr fArg = arg.port1;
			i64VariablePtr sArg = arg.port2;
			result = fArg && sArg;
			return true;
		}
		bool logicOrF(
			const operationArgument& arg)
		{
			i64VariablePtr result = arg.port0;
			i64VariablePtr fArg = arg.port1;
			i64VariablePtr sArg = arg.port2;
			result = fArg || sArg;
			return true;
		}
		bool incrementF(
			const operationArgument& arg)
		{
			i64VariablePtr Arg= arg.port0;
			Arg = Arg.get() + 1;
			return true;
		}
		bool decrementF(
			const operationArgument& arg)
		{
			i64VariablePtr Arg = arg.port0;
			Arg = Arg.get() - 1;
			return true;
		}
	}

	class mathOperationPtr : public existing<nodePtr>
	{
	public:
		static const nodeOperationSet operSet;
		mathOperationPtr(const existing<nodePtr>& eptr)
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
		operationData* opDataPtr()
		{
			return getDataPtr<operationData>();
		}
	};
	namespace mathOperationO
	{
		using namespace mathOperationF;
		const conRule vv(cr::AnyVariable, 0, 1, cr::AnyVariable, 1, 0);
		const conRule vvv(cr::AnyVariable, 0, 1, cr::AnyVariable, 1, 0, cr::AnyVariable, 1, 0);
		const conRule ivv(cr::I64, 0, 1, cr::AnyVariable, 1, 0, cr::AnyVariable, 1, 0);
		const conRule i(cr::I64, 1, 1);
		const conRule ii(cr::I64, 0, 1, cr::I64, 1, 0);
		const conRule iii(cr::I64, 0, 1, cr::I64, 1, 0, cr::I64, 1, 0);

		const operationProperties moProp{ false };

		const operation assigmentO{ L"=", vv, moProp, assigmentF };
		const operation unaryMinusO{ L"-", vv, moProp, unaryMinusF };
		const operation additionO{ L"+", vvv, moProp, additionF };
		const operation subtractionO{ L"-", vvv, moProp, subtractionF };
		const operation multiplicationO{ L"*", vvv, moProp, multiplicationF };
		const operation	divisionO{ L"/", vvv, moProp, divisionF };
		const operation equalO{ L"==", ivv, moProp, equalF };
		const operation notEqualO{ L"!=", ivv, moProp, notEqualF };
		const operation lessO{ L"<", ivv, moProp, lessF };
		const operation greaterO{ L">", ivv, moProp, greaterF };
		const operation lessOrEqualO{ L"<=", ivv, moProp, lessOrEqualF };
		const operation greaterOrEqualO{ L">=", ivv, moProp, greaterOrEqualF };
		const operation logicNegationO{ L"!", ii, moProp, logicNegationF };
		const operation logicAndO{ L"&&", iii, moProp, logicAndF };
		const operation logicOrO{ L"||", iii, moProp, logicOrF };
		const operation IncrementO{ L"++", i, moProp, incrementF };
		const operation DecrementO{ L"++", i, moProp, decrementF };

	}
	const nodeOperationSet mathOperationPtr::operSet
	{
		mathOperationO::assigmentO,
		mathOperationO::unaryMinusO,
		mathOperationO::additionO,
		mathOperationO::subtractionO,
		mathOperationO::multiplicationO,
		mathOperationO::divisionO,
		mathOperationO::equalO,
		mathOperationO::notEqualO,
		mathOperationO::lessO,
		mathOperationO::greaterO,
		mathOperationO::lessOrEqualO,
		mathOperationO::greaterOrEqualO,
		mathOperationO::logicNegationO,
		mathOperationO::logicAndO,
		mathOperationO::logicOrO,
		mathOperationO::IncrementO,
		mathOperationO::DecrementO
	};

}
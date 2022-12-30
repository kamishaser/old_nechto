#pragma once
#include "nodePtr.h"

#include <array>
#include <string>
namespace nechto
{
	using ntV = variablePtr;
	using ntI64 = i64VariablePtr;
	using ntF64 = f64VariablePtr;
	using ntPo = pointerPtr;
	using ntSP = simplePointerPtr;
	using ntPI = portIteratorPtr;
	using ntGI = groupIteratorPtr;
	using ntI = iteratorPtr;
	using ntT = textPtr;
	using ntG = groupPtr;
	using ntO = nonTypedObjectPtr;
	using ntMa = mathOperatorPtr;
	using ntMe = methodPtr;

	struct connectionRule
	{
		//корректный тип подключения
		enum conType
		{
			Any,//что угодно без проверки. Указатели игрнорятся
			I64Variable,
			F64Variable,
			AnyVariable,
			AnyPointer,
			AnyPointer_NoTransit,
			SimplePointer_NoTransit,
			PortIterator_NoTransit,
			GroupIterator_NoTransit,
			AnyIterator_NoTransit,
			Text,
			Group,
			Object,
			Math,
			Method
		};
		//тип использования соединения (на запись и чтения данных
		enum useType
		{
			None,
			readOnly,
			writeOnly,
			readOrWrite,
			readWithoutCheck
		};
		
		std::array<conType, 4> cType;
		std::array<useType, 4> uType;

		static const connectionRule NoneCR;//отсутствие правил подключения

		static bool transit(conType type)
		{
			return (type < AnyPointer_NoTransit) || (type > AnyIterator_NoTransit);
		}

		connectionRule(
			conType ct0 = Any, useType ut0 = None,
			conType ct1 = Any, useType ut1 = None,
			conType ct2 = Any, useType ut2 = None,
			conType ct3 = Any, useType ut3 = None)
			:cType{ ct0, ct1, ct2, ct3 },
			uType{ ut0, ut1, ut2, ut3 } {}

		nodePtr getConnection(existing<nodePtr> v1, int number)const;
		
		bool check(nodePtr v1) const
		{
			for (int i = 0; i < 4; ++i)
				if (!getConnection(v1, i).exist() && cType[i] != conType::Any)
					return false;
			return true;
		}
	};
	using conRule = connectionRule;
	const connectionRule conRule::NoneCR;//отсутствие правил подключения

	struct operation
	{
		connectionRule cRule;
		bool(*operationPtr)(nodePtr, nodePtr, nodePtr);
		static const operation None;

		operation(connectionRule cr = conRule::NoneCR, 
			bool(*op)(nodePtr, nodePtr, nodePtr) = nullptr)
			:cRule(cr), operationPtr(op) {}
		//получение типа подключения в зависимости от типа аргумента
		template<typedNodePtr conType>
		static constexpr conRule::conType getConType()
		{
			if constexpr (std::is_same<conType, i64VariablePtr>())
				return conRule::conType::I64Variable;
			if constexpr (std::is_same<conType, f64VariablePtr>())
				return conRule::conType::F64Variable;
			if constexpr (std::is_same<conType, variablePtr>())
				return conRule::conType::AnyVariable;
			if constexpr (std::is_same<conType, pointerPtr>())
				return conRule::conType::AnyPointer_NoTransit;
			if constexpr (std::is_same<conType, simplePointerPtr>())
				return conRule::conType::SimplePointer_NoTransit;
			if constexpr (std::is_same<conType, iteratorPtr>())
				return conRule::conType::AnyIterator_NoTransit;
			if constexpr (std::is_same<conType, portIteratorPtr>())
				return conRule::conType::PortIterator_NoTransit;
			if constexpr (std::is_same<conType, groupIteratorPtr>())
				return conRule::conType::GroupIterator_NoTransit;
			if constexpr (std::is_same<conType, groupPtr>())
				return conRule::conType::Group;
			if constexpr (std::is_same<conType, methodPtr>())
				return conRule::conType::Method;
			if constexpr (std::is_same<conType, mathOperatorPtr>())
				return conRule::conType::Math;
			if constexpr (std::is_same<conType, nonTypedObjectPtr>())
				return conRule::conType::Object;
			return conRule::conType::Any;
		}
		template<typedNodePtr conType>
		static constexpr conRule::useType getUseType()
		{
			if constexpr (std::is_same<conType, nullNodePtrT>())
				return conRule::useType::None;
			if constexpr (std::is_const<conType>())
				return conRule::useType::readOnly;
			return conRule::useType::readOrWrite;
		}

		//обёртка для преобразования аргументов в nodePtr
		template<typedNodePtr con0Type, typedNodePtr con1Type, typedNodePtr con2Type,
		bool(*op)(con0Type, con1Type, con2Type)>
		static bool wrapper3(nodePtr con0, nodePtr con1, nodePtr con2)
		{
			return op(con0, con1, con2);
		}
		template<class con0Type, class con1Type,
			bool(*op)(con0Type, con1Type)>
		static bool wrapper2(nodePtr con0, nodePtr con1, nodePtr con2)
		{
			return op(con0, con1);
		}
		template<typedNodePtr con0Type,
			bool(*op)(con0Type)>
		static bool wrapper1(nodePtr con0, nodePtr con1, nodePtr con2)
		{
			return op(con0);
		}

		bool operate(nodePtr v1) const
		{
			nodePtr n[3];
			for (int i = 0; i < 3; ++i)
			{
				n[i] = cRule.getConnection(v1, i);
				if (!n[i].exist() && cRule.uType[i] != connectionRule::None)
					return false;
			}
			if (operationPtr == nullptr)
				return false;
			return operationPtr(n[0], n[1], n[2]);
		}

		template<typedNodePtr con0Type, typedNodePtr con1Type, typedNodePtr con2Type, 
			bool(*op)(con0Type, con1Type, con2Type)>
		constexpr static operation wrap()
		{
			operation oper;
			oper.cRule = connectionRule
			(
				getConType<con0Type>(), getUseType<con0Type>(),
				getConType<con1Type>(), getUseType<con1Type>(),
				getConType<con2Type>(), getUseType<con2Type>()
			);
			oper.operationPtr = wrapper3<con0Type, con1Type, con2Type, op>;
			return oper;
		}
		template<typedNodePtr con0Type, typedNodePtr con1Type,
			bool(*op)(con0Type, con1Type)>
		constexpr static operation wrap()
		{
			operation oper;
			oper.cRule = connectionRule
			(
				getConType<con0Type>(), getUseType<con0Type>(),
				getConType<con1Type>(), getUseType<con1Type>()
			);
			oper.operationPtr = wrapper2<con0Type, con1Type, op>;
			return oper;
		}
		template<typedNodePtr con0Type, bool(*op)(con0Type)>
		constexpr static operation wrap()
		{
			operation oper;
			oper.cRule = connectionRule
			(
				getConType<con0Type>(), getUseType<con0Type>()
			);
			oper.operationPtr = wrapper1<con0Type, op>;
			return oper;
		}

		constexpr void operator=(const operation& op)
		{
			cRule = op.cRule;
			operationPtr = op.operationPtr;
		}
	}; 
	const operation operation::None = operation();
}
#pragma once
#include "nodePtr.h"
#include "pointerPtr.h"
#include "variablePtr.h"
#include <array>
namespace nechto
{

	struct connectionRule
	{
		//корректный тип подключения
		enum conType
		{
			Any,
			I64Variable,
			F64Variable,
			AnyVariable,
			AnyPointer,
			AnyPointer_NoTransit,
			SimplePointer_NoTransit,
			ConIterator_NoTransit,
			GroupIterator_NoTransit,
			AnyIterator_NoTransit,
			Text,
			Group,
			ExternalObject,
			Math,
			Method
		};
		enum useType
		{
			None,
			readOnly,
			readOrWrite,
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

		nodePtr getConnection(existing<nodePtr> v1, int number)const
		{
			nodePtr vCon = v1.connection(number);
			char type = vCon.type();
			bool transited = false;
			if (!vCon.exist())
				return nullptr;
			if (type == nodeT::Pointer && transit(cType[number]))
			{
				vCon = pointerPtr(vCon).follow();
				transited = true;
				if (!vCon.exist())
					return nullptr;
				type = vCon.type();
			}
			char subtype = vCon.subtype();
			switch (cType[number])
			{
			case nechto::connectionRule::Any:
				break;
			case nechto::connectionRule::I64Variable:
				if (type != nodeT::Variable || !subtype)
					return nullptr;
				break;
			case nechto::connectionRule::F64Variable:
				if (type != nodeT::Variable || subtype)
					return nullptr;
				break;
			case nechto::connectionRule::AnyVariable:
				if (type != nodeT::Variable)
					return nullptr;
				break;
			case nechto::connectionRule::AnyPointer:
				if (!transited)
					return nullptr;
				break;
			case nechto::connectionRule::AnyPointer_NoTransit:
				if (type != nodeT::Pointer)
					return nullptr;
				break;
			case nechto::connectionRule::SimplePointer_NoTransit:
				if (type != nodeT::Pointer || subtype != pointerT::Simple)
					return nullptr;
				break;
			case nechto::connectionRule::ConIterator_NoTransit:
				if (type != nodeT::Pointer || subtype != pointerT::ConIter)
					return nullptr;
				break;
			case nechto::connectionRule::GroupIterator_NoTransit:
				if (type != nodeT::Pointer || subtype != pointerT::GroupIter)
					return nullptr;
				break;
			case nechto::connectionRule::AnyIterator_NoTransit:
				if (type != nodeT::Pointer || 
					subtype == pointerT::Simple)
					return nullptr;
				break;
			case nechto::connectionRule::Text:
				if (type != nodeT::Text)
					return nullptr;
				break;
			case nechto::connectionRule::Group:
				if (type != nodeT::Group)
					return nullptr;
				break;
			case nechto::connectionRule::ExternalObject:
				if (type != nodeT::ExternalObject)
					return nullptr;
				break;
			case nechto::connectionRule::Method:
				if (type != nodeT::Method)
					return nullptr;
				break;
			default:
				assert(false);
				break;
			}
			return vCon;
		}
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
		//конструктор с автоматической генерацией правила
		template<typedNodePtr con0Type, typedNodePtr con1Type, typedNodePtr con2Type>
		constexpr operation(bool(*op)(con0Type, con1Type, con2Type))
			:cRule
			(
				getConType<con0Type>(), getUseType<con0Type>(),
				getConType<con1Type>(), getUseType<con1Type>(),
				getConType<con2Type>(), getUseType<con2Type>()
			),
			operationPtr(wrapper3<con0Type, con1Type, con2Type, op>){}
		template<typedNodePtr con0Type, typedNodePtr con1Type>
		constexpr operation(bool(*op)(con0Type, con1Type))
			:cRule
			(
				getConType<con0Type>(), getUseType<con0Type>(),
				getConType<con1Type>(), getUseType<con1Type>()
			),
			operationPtr(wrapper2<con0Type, con1Type, op>) {}
		template<typedNodePtr con0Type>
		constexpr operation(bool(*op)(con0Type))
			:cRule
			(
				getConType<con0Type>(), getUseType<con0Type>()
			),
			operationPtr(wrapper2<con0Type, op>) {}

		//получение типа подключения в зависимости от типа аргумента
		template<typedNodePtr conType>
		static conRule::conType getConType()
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
			if constexpr (std::is_same<conType, conIteratorPtr>())
				return conRule::conType::ConIterator_NoTransit;
			if constexpr (std::is_same<conType, groupIteratorPtr>())
				return conRule::conType::GroupIterator_NoTransit;
			if constexpr (std::is_same<conType, groupPtr>())
				return conRule::conType::Group;
			if constexpr (std::is_same<conType, methodPtr>())
				return conRule::conType::Method;
			if constexpr (std::is_same<conType, mathPtr>())
				return conRule::conType::Math;
			if constexpr (std::is_same<conType, exteranalObjectPtr>())
				return conRule::conType::ExternalObject;
			return conRule::conType::Any;
		}
		template<typedNodePtr conType>
		static conRule::useType getUseType()
		{
			if constexpr (std::is_same<conType, nullNodePtrT>())
				return conRule::useType::None;
			if constexpr (std::is_const<conType>())
				return conRule::useType::readOnly;
			return conRule::useType::readOrWrite;
		}

		//обёртка для преобразования аргументов в nodePtr
		template<typedNodePtr con0Type, typedNodePtr con1Type, typedNodePtr con2Type,
		bool(*operation)(con0Type, con1Type, con2Type)>
		bool wrapper3(nodePtr con0, nodePtr con1, nodePtr con2)
		{
			return operation(con0, con1, con2);
		}
		template<typedNodePtr con0Type, typedNodePtr con1Type,
			bool(*operation)(con0Type, con1Type)>
		bool wrapper2(nodePtr con0, nodePtr con1, nodePtr con2)
		{
			return operation(con0, con1);
		}
		template<typedNodePtr con0Type,
			bool(*operation)(con0Type)>
		bool wrapper1(nodePtr con0, nodePtr con1, nodePtr con2)
		{
			return operation(con0);
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
	}; 
	const operation operation::None = operation();
}
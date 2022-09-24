#pragma once
#include "node.h"
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
			Method
		};
		enum useType
		{
			None,
			Input,
			Output,
			In_Output
		};
		
		std::array<conType, 4> cType;
		std::array<useType, 4> uType;
		std::array<nodeEvent, 4> additionCheck;

		static const connectionRule NoneCR;//отсутствие правил подключения

		static bool transit(conType type)
		{
			return (type < AnyPointer_NoTransit) || (type > AnyIterator_NoTransit);
		}

		connectionRule(
			conType ct0 = Any, useType ut0 = None, nodeEvent ach0 = nullptr,
			conType ct1 = Any, useType ut1 = None, nodeEvent ach1 = nullptr,
			conType ct2 = Any, useType ut2 = None, nodeEvent ach2 = nullptr,
			conType ct3 = Any, useType ut3 = None, nodeEvent ach3 = nullptr)
			:cType(ct0, ct1, ct2, ct3), 
			uType(ut0, ut1, ut2, ut3), 
			additionCheck(ach0, ach1, ach2, ach3) {}

		nodePtr getConnection(nodePtr v1, int number)const
		{
			nodePtr vCon = v1->connection[number];
			char type = vCon->getType();
			bool transited = false;
			if (!vCon.exist())
				return nullNodePtr;
			if (type == node::Pointer && transit(cType[number]))
			{
				vCon = pointer::follow(vCon);
				transited = true;
				if (!vCon.exist())
					return nullNodePtr;
				type = vCon->getType();
			}
			char subtype = vCon->getSubtype();
			switch (cType[number])
			{
			case nechto::connectionRule::Any:
				break;
			case nechto::connectionRule::I64Variable:
				if (type != node::Variable || !subtype)
					return nullNodePtr;
				break;
			case nechto::connectionRule::F64Variable:
				if (type != node::Variable || subtype)
					return nullNodePtr;
				break;
			case nechto::connectionRule::AnyVariable:
				if (type != node::Variable)
					return nullNodePtr;
				break;
			case nechto::connectionRule::AnyPointer:
				if (!transited)
					return nullNodePtr;
				break;
			case nechto::connectionRule::AnyPointer_NoTransit:
				if (type != node::Pointer)
					return nullNodePtr;
				break;
			case nechto::connectionRule::SimplePointer_NoTransit:
				if (type != node::Pointer || subtype != pointer::Simple)
					return nullNodePtr;
				break;
			case nechto::connectionRule::ConIterator_NoTransit:
				if (type != node::Pointer || subtype != pointer::ConIter)
					return nullNodePtr;
				break;
			case nechto::connectionRule::GroupIterator_NoTransit:
				if (type != node::Pointer || subtype != pointer::GroupIter)
					return nullNodePtr;
				break;
			case nechto::connectionRule::AnyIterator_NoTransit:
				if (type != node::Pointer || 
					subtype == pointer::Simple)
					return nullNodePtr;
				break;
			case nechto::connectionRule::Text:
				if (type != node::Text)
					return nullNodePtr;
				break;
			case nechto::connectionRule::Group:
				if (type != node::Group)
					return nullNodePtr;
				break;
			case nechto::connectionRule::ExternalObject:
				if (type != node::ExternalObject)
					return nullNodePtr;
				break;
			case nechto::connectionRule::Method:
				if (type != node::Method)
					return nullNodePtr;
				break;
			default:
				assert(false);
				break;
			}
			if (additionCheck[number] != nullptr)
				if (!additionCheck[number](v1))
					return nullNodePtr;
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
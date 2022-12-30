#pragma once
#include "connectionRule.h"
#include "pointerPtr.h"
#include "variablePtr.h"
#include "mathOperator.h"
#include "text.h"
#include "group.h"
#include "method.h"
#include "object.h"

namespace nechto
{
	nodePtr connectionRule::getConnection(existing<nodePtr> v1, int number)const
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
		case nechto::connectionRule::PortIterator_NoTransit:
			if (type != nodeT::Pointer || subtype != pointerT::PortIter)
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
		case nechto::connectionRule::Object:
			if (type != nodeT::Object)
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
}
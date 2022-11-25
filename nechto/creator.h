#pragma once
#include "nodePtr.h"
#include "connectionIterator.h"

#include "hubPtr.h"
#include "variablePtr.h"
#include "pointerPtr.h"
#include "groupPtr.h"
#include "mathOperator.h"
#include "externalObject.h"
#include "condition.h"
#include "method.h"

namespace nechto
{
	class creator
	{
		static existing<nodePtr> allocate(char type, char subtype = 0)
		{
			nodePtr node = nodeStorage::terminal.allocate();
			node.node()->type = type;
			node.node()->subtype = subtype;
			return node;
		}
		static void deallocate(existing<nodePtr> node)
		{
			nodeStorage::terminal.deallocate(node);
		}
	public:
		/*static existing<nodePtr> addNode(char type, char subtype)
		{
		
		}*/
		

		static hubPtr createHub()
		{
			existing<nodePtr> node = allocate(nodeT::Hub);
			return node;
		}
		static i64VariablePtr createI64()
		{
			existing<nodePtr> node = allocate(nodeT::Variable, 1);
			node.setData<i64>(0);
			return node;
		}
		static f64VariablePtr createF64()
		{
			existing<nodePtr> node = allocate(nodeT::Variable, 0);
			node.setData<f64>(0.);
			return node;
		}
		static simplePointerPtr createSimplePointer()
		{
			existing<nodePtr> node = allocate(nodeT::Pointer, pointerT::Simple);
			return node;
		}
		static portIteratorPtr createConIterator()
		{
			existing<nodePtr> node = allocate(nodeT::Pointer, pointerT::ConIter);
			iteratorPtr(node).setHPPair(hubPosPair(nullptr, 0));
			return node;
		}
		static groupIteratorPtr createGroupIterator()
		{
			existing<nodePtr> node = allocate(nodeT::Pointer, pointerT::GroupIter);
			iteratorPtr(node).setHPPair(hubPosPair(nullptr, 0));
			return node;
		}
		static mathOperatorPtr createMathOperator(char operationType)
		{
			existing<nodePtr> node = allocate(nodeT::MathOperator, operationType);
			return node;
		}
		static methodPtr createMethod(char operationType)
		{
			existing<nodePtr> node = allocate(nodeT::Method, operationType);
			return node;
		}
		static conditionPtr createCondition()
		{
			existing<nodePtr> node = allocate(nodeT::Condition);
			return node;
		}
		static nonTypedExternalObjectPtr createExternalObject(
			externalObject* object = nullptr, char own = 1)
		{
			existing<nodePtr> node = allocate(nodeT::ExternalObject, own);
			nonTypedExternalObjectPtr(node).setObjectPtr(object);
			return node;
		}
		static groupPtr createGroup(bool own = true)
		{
			existing<nodePtr> node = allocate(nodeT::Group, own);
			hubPtr hub = createHub();
			hub.setPrevious(hub);
			hub.node()->hubPort = hub;
			groupPtr(node).setFirstGroupHub(hub);
			return node;
		}

		static void deleteNode(char type)
		{
			//...
			//deallocate(type);
		}
		static void deleteHub(hubPtr hub)
		{
			deallocate(hub);
		}
	};
}
#pragma once
#include "nodePtr.h"
#include "connectionIterator.h"

#include "hubPtr.h"
#include "variablePtr.h"
#include "pointerPtr.h"
#include "groupPtr.h"

namespace nechto
{
	class creator
	{
		static existing<nodePtr> allocate()
		{
			return nodePtr(nodeStorage::terminal.allocate());
		}
		static void deallocate(existing<nodePtr> node)
		{
			nodeStorage::terminal.deallocate(node);
		}
		static void setTypeAndSubtype(
			existing<nodePtr> node, char type, char subtype = 0)
		{
			node.node()->type = type;
			node.node()->subtype = subtype;
		}
	public:
		/*static existing<nodePtr> addNode(char type, char subtype)
		{
		
		}*/
		static void deleteNode(char type)
		{
			//...
			deallocate(type);
		}

		static hubPtr createHub()
		{
			nodePtr hub = allocate();
			setTypeAndSubtype(hub, nodeT::Hub);
			return hub;
		}
		static i64VariablePtr createI64()
		{
			nodePtr i64Var = allocate();
			setTypeAndSubtype(i64Var, nodeT::Variable, 1);
			i64Var.setData<i64>(0);
			return i64Var;
		}
		static f64VariablePtr createF64()
		{
			nodePtr f64Var = allocate();
			setTypeAndSubtype(f64Var, nodeT::Variable, 0);
			f64Var.setData<f64>(0.);
			return f64Var;
		}
		static simplePointerPtr createSimplePointer()
		{
			nodePtr sPointer = allocate();
			setTypeAndSubtype(sPointer, nodeT::Pointer, pointerT::Simple);
			return sPointer;
		}
		static conIteratorPtr createConIterator()
		{
			nodePtr conIterator = allocate();
			setTypeAndSubtype(conIterator, nodeT::Pointer, pointerT::ConIter);
			iteratorPtr(conIterator).setHPPair(hubPosPair(nullptr, 0));
			return conIterator;
		}
		static groupIteratorPtr createGroupIterator()
		{
			nodePtr	groupIterator = allocate();
			setTypeAndSubtype(groupIterator, nodeT::Pointer, pointerT::GroupIter);
			iteratorPtr(groupIterator).setHPPair(hubPosPair(nullptr, 0));
			return groupIterator;
		}
		

	};
}
#pragma once
#include "nodePtr.h"
#include "connectionIterator.h"

#include "hubPtr.h"
#include "variablePtr.h"
#include "pointerPtr.h"
#include "groupPtr.h"
#include "mathOperator.h"
#include "object.h"
#include "condition.h"
#include "method.h"
#include "text.h"

namespace nechto
{
	class creator
	{
		friend class hubManager;
	public:

		static hubPtr createHub(bool groupHub)
		{
			existing<nodePtr> node = allocate(nodeT::Hub, groupHub);
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
		static variablePtr createVariable(char subtype)
		{
			if (subtype)
				return createI64();
			else
				return createF64();
		}
		static simplePointerPtr createSimplePointer()
		{
			existing<nodePtr> node = allocate(nodeT::Pointer, pointerT::Simple);
			return simplePointerPtr(node);
		}
		static portIteratorPtr createPortIterator()
		{
			existing<nodePtr> node = allocate(nodeT::Pointer, pointerT::PortIter);
			iteratorPtr(node).setHPPair(hubPosPair(nullptr, 0));
			return portIteratorPtr(node);
		}
		static groupIteratorPtr createGroupIterator()
		{
			existing<nodePtr> node = allocate(nodeT::Pointer, pointerT::GroupIter);
			iteratorPtr(node).setHPPair(hubPosPair(nullptr, 0));
			return groupIteratorPtr(node);
		}
		static pointerPtr createPointer(char subtype)
		{
			switch (subtype)
			{
			case nechto::pointerT::Simple:
				return createSimplePointer();
			case nechto::pointerT::PortIter:
				return createPortIterator();
			case nechto::pointerT::GroupIter:
				return createPortIterator();
			default:
				assert(false);
			}
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
		static nonTypedObjectPtr createObject(char uniqueOwner,
			object* object = nullptr)
		{
			existing<nodePtr> node = allocate(nodeT::Object, uniqueOwner);
			nonTypedObjectPtr(node).setObjectPtr(object);
			return node;
		}
		static textPtr createText(char own)
		{
			existing<nodePtr> node = allocate(nodeT::Text, own);
			textPtr(node).setData<std::wstring*>(nullptr);
			return node;
		}
		static groupPtr createGroup(bool own = true)
		{
			existing<nodePtr> node = allocate(nodeT::Group, own);
			hubPtr hub = createHub(1);
			hub.connect(hub);
			groupPtr(node).setFirstGroupHub(hub);
			return node;
		}
		
		static void deleteNode(existing<nodePtr> node)
		{
			switch (node.type())
			{
			case nechto::nodeT::Deleted:
				break;
			case nechto::nodeT::Hub:
				assert(false);//ошибка! ’абы удал€ютс€ только через hubManager
				break;
			case nechto::nodeT::Group:
				disconnectAllGroup(node);
				deleteAllGroupHubs(node);
				break;
			case nechto::nodeT::Pointer:
				break;
			case nechto::nodeT::Variable:
				break;
			case nechto::nodeT::Object:
				if (nonTypedObjectPtr(node).isUniqueOwner())
					delete nonTypedObjectPtr(node).getObjectPtr();
				break;
			case nechto::nodeT::Text:
				textPtr(node).reset();
				break;
			case nechto::nodeT::Method:
				break;
			case nechto::nodeT::NodeOperator:
				break;
			default:
				break;
			}
			node.node()->data = 0;
			node.node()->type = 0;
			node.node()->subtype = 0;
			disconnectAll(node);
			deleteAllHubs(node);
		}
		static nodePtr createNode(char type, char subtype)
		{
			switch (type)
			{
			case nechto::nodeT::Group:
				return createGroup(subtype);
			case nechto::nodeT::Pointer:
				return createPointer(subtype);
			case nechto::nodeT::Variable:
				return createVariable(subtype);
			case nechto::nodeT::Object:
				return createObject(subtype);
			case nechto::nodeT::Text:
				return createText(subtype);
			case nechto::nodeT::MathOperator:
				return createMathOperator(subtype);
			case nechto::nodeT::Condition:
				return createCondition();
			case nechto::nodeT::Method:
				return createMethod(subtype);
			default:
				assert(false);
			}
		}
	private:

		static void disconnectAll(existing<nodePtr> node);
		static void disconnectAllGroup(groupPtr group);
		static existing<nodePtr> allocate(char type, char subtype = 0)
		{
			nodePtr node = nodeStorage::terminal.allocate();
			node.node()->type = type;
			node.node()->subtype = subtype;
			return node;
		}
		static void deallocate(existing<nodePtr> node)
		{
			for (int i = 0; i < 4; ++i)
				assert(!node.connection(i).exist());
			nodeStorage::terminal.deallocate(node);
		}
		static void deleteHub(hubPtr hub)
		{
			deallocate(hub);
		}
		static void deleteAllGroupHubs(groupPtr group)
		{
			const hubPtr firstHub = group.firstGroupHub();//первый хаб
			nodePtr currentHub = firstHub;
			do
			{
				nodePtr next = currentHub.hub();
				deleteHub(currentHub);
				currentHub = next;
			} while (currentHub != firstHub);
		}
		static void deleteAllHubs(existing<nodePtr> node)
		{
			nodePtr currentHub = node.hub();
			while (currentHub.exist())
			{
				nodePtr next = currentHub.hub();
				deleteHub(currentHub);
				currentHub = next;
			} 
		}
	};
}
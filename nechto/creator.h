#pragma once
#include "nodePtr.h"
#include "connectionIterator.h"

#include "hubPtr.h"
#include "variablePtr.h"
#include "iteratorPtr.h"
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
		static std::array<i64, 255> numberOfTypedNodes;
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
		static portIteratorPtr createPortIterator()
		{
			existing<nodePtr> node = allocate(nodeT::Iterator, iteratorT::PortIter);
			iteratorPtr(node).setHPPair(hubPosPair(nullptr, 0));
			return portIteratorPtr(node);
		}
		static groupIteratorPtr createGroupIterator()
		{
			existing<nodePtr> node = allocate(nodeT::Iterator, iteratorT::GroupIter);
			iteratorPtr(node).setHPPair(hubPosPair(nullptr, 0));
			return groupIteratorPtr(node);
		}
		static iteratorPtr createIterator(char subtype)
		{
			switch (subtype)
			{
			case nechto::iteratorT::PortIter:
				return createPortIterator();
			case nechto::iteratorT::GroupIter:
				return createGroupIterator();
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
			groupPtr(node).setSize(1);
			return node;
		}
		
		static void deleteNode(existing<nodePtr> node)
		{
			switch (node.type())
			{
			case nechto::nodeT::Deleted:
				break;
			case nechto::nodeT::Hub:
				assert(false);//������! ���� ��������� ������ ����� hubManager
				break;
			case nechto::nodeT::Group:
				disconnectAllGroup(node);
				deleteAllGroupHubs(node);
				break;
			case nechto::nodeT::Iterator:
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
			disconnectAll(node);
			deleteAllHubs(node);
			deallocate(node);
		}
		static nodePtr createNode(char type, char subtype)
		{
			switch (type)
			{
			case nechto::nodeT::Group:
				return createGroup(subtype);
			case nechto::nodeT::Iterator:
				return createIterator(subtype);
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
			++(numberOfTypedNodes[static_cast<int>(type)]);
			node.node()->type = type;
			node.node()->subtype = subtype;
			return node;
		}
		static void deallocate(existing<nodePtr> node)
		{
			--(numberOfTypedNodes[static_cast<int>(node.type())]);
			node.node()->data = 0;
			node.node()->type = 0;
			node.node()->subtype = 0;
			for (int i = 0; i < 4; ++i)
				assert(!node.connection(i).exist());
			nodeStorage::terminal.deallocate(node);
		}
		static void deleteHub(hubPtr hub)
		{
			deallocate(hub);
		}
		static hubPtr createHub(bool groupHub)
		{
			existing<nodePtr> node = allocate(nodeT::Hub, groupHub);
			return node;
		}
		static void deleteAllGroupHubs(groupPtr group)
		{
			const hubPtr firstHub = group.firstGroupHub();//������ ���
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
	std::array<i64, 255> creator::numberOfTypedNodes{ 0 };
}
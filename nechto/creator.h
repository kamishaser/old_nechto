#pragma once
#include "nodePtr.h"
#include "connectionPointer.h"

#include "hubPtr.h"
#include "variablePtr.h"
#include "pointerPtr.h"
#include "groupPtr.h"
#include "MathOperation.h"
#include "entity.h"
#include "condition.h"
#include "method.h"
#include "text.h"
#include "textOperation.h"
#include "operatorManagement.h"
#include "hubManagement.h"
#include "pointerMovementOperation.h"
#include "operator.h"
#include "readingOperation.h"
#include "creationOperation.h"
#include "connectionOperation.h"
#include "vectorOperation.h"
#include "vector.h"
#include "struct.h"

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
		static variablePtr createVariable(unsigned char subtype)
		{
			if (subtype)
				return createI64();
			else
				return createF64();
		}
		static portPointerPtr createPortPointer()
		{
			existing<nodePtr> node = allocate(nodeT::Pointer, pointerT::PortPointer);
			pointerPtr(node).setHPPair(hubPosPair(nullptr, 0));
			return portPointerPtr(node);
		}
		static groupPointerPtr createGroupPointer()
		{
			existing<nodePtr> node = allocate(nodeT::Pointer, pointerT::GroupPointer);
			pointerPtr(node).setHPPair(hubPosPair(nullptr, 0));
			return groupPointerPtr(node);
		}
		static pointerPtr createPointer(unsigned char subtype)
		{
			switch (subtype)
			{
			case nechto::pointerT::PortPointer:
				return createPortPointer();
			case nechto::pointerT::GroupPointer:
				return createGroupPointer();
			default:
				assert(false);
			}
		}
		static mathOperationPtr createMathOperation(unsigned char operationType)
		{
			existing<nodePtr> node = allocate(nodeT::MathOperation, operationType);
			return node;
		}
		static methodPtr createMethod(unsigned char operationType)
		{
			existing<nodePtr> node = allocate(nodeT::Method, operationType);
			return node;
		}
		static conditionPtr createCondition()
		{
			existing<nodePtr> node = allocate(nodeT::Condition);
			return node;
		}
		static textPtr createText(bool own)
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
		/////////////////////////////////////////////
		static structPtr createStruct(unsigned char subtype)
		{
			existing<nodePtr> node = allocate(nodeT::Struct, subtype);
			return node;
		}
		static vectorPtr createVector(unsigned char subtype)
		{
			existing<nodePtr> node = allocate(nodeT::Vector, subtype);
			return node;
		}
		static operatorPtr createOperator(unsigned char subtype)
		{
			existing<nodePtr> node = allocate(nodeT::Operator, subtype);
			return node;
		}
		static vectorOperationPtr createVectorOperation(unsigned char subtype)
		{
			existing<nodePtr> node = allocate(nodeT::VectorOperation, subtype);
			return node;
		}
		static connectionOperationPtr createConnectionOperation(unsigned char subtype)
		{
			existing<nodePtr> node = allocate(nodeT::ConnectionOperation, subtype);
			return node;
		}
		static creationOperationPtr createCreationOperation(unsigned char subtype)
		{
			existing<nodePtr> node = allocate(nodeT::CreationOperation, subtype);
			return node;
		}
		static pointerMovementOperationPtr createPointerationOperation(unsigned char subtype)
		{
			existing<nodePtr> node = allocate(nodeT::PointerationOperation, subtype);
			return node;
		}
		static readingOperationPtr createReadingOperation(unsigned char subtype)
		{
			existing<nodePtr> node = allocate(nodeT::ReadingOperation, subtype);
			return node;
		}
		static textOperationPtr createTextOperation(unsigned char subtype)
		{
			existing<nodePtr> node = allocate(nodeT::TextOperation, subtype);
			return node;
		}
		static hubManagementPtr createHubManagement(unsigned char subtype)
		{
			existing<nodePtr> node = allocate(nodeT::HubManagement, subtype);
			return node;
		}
		static operatorManagementPtr createOperatorManagement(unsigned char subtype)
		{
			existing<nodePtr> node = allocate(nodeT::OperatorManagement, subtype);
			return node;
		}
		static entityPtr createEntity(unsigned char subtype, entityInterface* ent)
		{
			existing<nodePtr> node = allocate(nodeT::Entity, subtype);
			if (ent == nullptr)
				node.setData(nullptr);
			else
				ent->connect(node);
			return node;
		}
		/*static TYPENAMEPtr createTYPENAME(unsigned char subtype)
		{
			existing<nodePtr> node = allocate(nodeT::TYPENAME, subtype);
			return node;
		}*/
		
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
			case nechto::nodeT::Entity:
				if (entityPtr(node).isOneSideLink())
					delete entityPtr(node).getEntityPtr();
				break;
			case nechto::nodeT::Text:
				textPtr(node).reset();
				break;
			case nechto::nodeT::Method:
				break;
			case nodeT::Struct:
				assert(false);
				break;
			case nodeT::Operator:
				assert(false);
				break;
			default:
				break;
			}
			disconnectAll(node);
			deleteAllHubs(node);
			deallocate(node);
		}
		static nodePtr createNode(unsigned char type, unsigned char subtype)
		{
			switch (type)
			{
			case nechto::nodeT::Group:
				return createGroup(subtype);
			case nechto::nodeT::Pointer:
				return createPointer(subtype);
			case nechto::nodeT::Variable:
				return createVariable(subtype);
			case nechto::nodeT::Entity:
				return createEntity(subtype);
			case nechto::nodeT::Text:
				return createText(subtype);
			case nechto::nodeT::MathOperation:
				return createMathOperation(subtype);
			case nechto::nodeT::Condition:
				return createCondition();
			//case nechto::nodeT::Method:
			//	return createMethod(subtype);
			default:
				assert(false);
			}
		}
	private:

		static void disconnectAll(existing<nodePtr> node);
		static void disconnectAllGroup(groupPtr group);
		static existing<nodePtr> allocate(unsigned char type, unsigned char subtype = 0)
		{
			nodePtr node = nodeStorage::terminal.allocate();
			++(numberOfTypedNodes[static_cast<int>(type)]);
			node.node()->type = type;
			node.node()->subtype = subtype;
			return node;
		}
		static void deallocate(existing<nodePtr> node)
		{
			--(numberOfTypedNodes[node.type()]);
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
	std::array<i64, 255> creator::numberOfTypedNodes{ 0 };
}
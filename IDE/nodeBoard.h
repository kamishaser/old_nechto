#pragma once
#include "visualNode.h"
#include "visualConnection.h"
#include "visualGroup.h"
namespace nechto::ide
{
	glm::vec2 randomPos(glm::vec2 border)
	{
		return glm::vec2
		(
			static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / border.x)),
			static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / border.y))
		);
	}
	glm::vec2 randomOffset(float max)
	{
		return glm::vec2
		(
			static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / max / 2)) - max,
			static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / max / 2)) - max
		);
	}
	struct nodeBoard : public namedExCon
	{
		nodeBoard()
			:namedExCon(L"nodeBoard")
		{
			for(int i = 0; i < 4; ++i)
				NumNumConnect(node(), creator::createGroup(), i, 0);
		}
		nodePtr vNodeGroup() const
		{
			return node().connection(0);
		}
		nodePtr vConnectionGroup() const
		{
			return node().connection(1);
		}
		nodePtr vGroupGroup() const
		{
			return node().connection(2);
		}
		//група различным образом помеченных нод и соединений
		nodePtr taggedGroup() const 
		{
			return node().connection(3);
		}
		
		void addNode(objectPtr<visualNode> vNode)
		{
			IterIterConnect(portIterator(vNode, 3), 
				firstEmptyGroupPort(vNodeGroup()));
		}
		void addConnection(objectPtr<visualConnection> vConnection)
		{
			IterIterConnect(portIterator(vConnection, 3),
				firstEmptyGroupPort(vConnectionGroup()));
		}
		void addGroup(objectPtr<visualGroup> vGroup)
		{
			IterIterConnect(portIterator(vGroup, 3),
				firstEmptyGroupPort(vGroupGroup()));
		}
		bool onThisBoard(objectPtr<visualNode> vNode)
		{
			return vNodeGroup() == vNode.connection(3);
		}
		bool onThisBoard(objectPtr<visualConnection> vCon)
		{
			return vConnectionGroup() == vCon.connection(3);
			//внимание!!!! Надо переделать
		}
		bool connected(objectPtr<visualNode> vNode1, objectPtr<visualNode> vNode2)
		{
			if (!onThisBoard(vNode1) || !onThisBoard(vNode2))
				return false;
			portIterator i1(vNode1);
			do
			{
				if (objectPtr<visualConnection>::match(i1.get()))
				{
					objectPtr<visualConnection> vscon(i1.get());
					if (vscon.connection(0) == vNode2 ||
						vscon.connection(1) == vNode2)
						return true;
				}
			} while (i1.stepForward());
			return false;
		}
		/*возвращает nullptr (интерпретируется в false) если
		на данной доске нет visualNode указывающей на эту ноду
		//иначе возвращает указатель на visualNode*/
		nodePtr visualized(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			groupIterator gi(vNodeGroup());
			do
			{
				if(objectPtr<visualNode>::match(gi.get()) && (gi.get().connection(0) == v1))
					return gi.get();
			} while (gi.stepForward());
			return nullptr;
		}
		const static std::wstring typeName;
		const static staticNodeOperationSet methodSet;
		const static connectionRule cRule;
		virtual const std::wstring& getTypeName() const override
		{
			return typeName;
		}
		virtual const operation& getMethod(char number)const override
		{
			return methodSet.getOperation(number);
		}
		virtual void serialize(std::vector<char>& buffer, existing<nodePtr> obj) const
		{
			buffer.clear();
		}
	};
	const std::wstring nodeBoard::typeName = L"nechtoIde.nodeBoard";
	const staticNodeOperationSet nodeBoard::methodSet
	{
		/*namedOperation(L"addNode", operation{
				connectionRule(
					conRule::ExternalObject, conRule::Input, nullptr,
					conRule::ExternalObject, conRule::Input, [](nodePtr v1)
					{
						if (v1->getData<externalObject*>() != nullptr)
							return false;
						return true;
					}),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				auto nBoard = v0->getData<nodeBoard*>();
				auto vNode = nBoard->addNode(v1);
				return true;
			}
			}),
		namedOperation(L"addConnection", operation{
				connectionRule(
					conRule::ExternalObject, conRule::Input, nullptr,
					conRule::ExternalObject, conRule::Input, [](nodePtr v1)
					{
						if (v1->getData<externalObject*>() != nullptr)
							return false;
						if (visualNode::
							getByNode(v1->connection[0]) == nullptr)
							return false;
						if (visualNode::
							getByNode(v1->connection[1]) == nullptr)
							return false;
						return true;
					}
					),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				auto nBoard = v0->getData<nodeBoard*>();
				auto vNode0 = v1->connection[0].load()->getData<visualNode*>();
				auto vNode1 = v1->connection[1].load()->getData<visualNode*>();
				auto vConnection = nBoard->addConnection(v1, vNode0, vNode1);
				return true;
			}}),
		namedOperation(L"getAllVConnections", operation{
				connectionRule(
					conRule::ExternalObject, conRule::Input, nullptr,
					conRule::ExternalObject, conRule::Input,
					[](nodePtr v1)
					{
						return (getObject<visualNode>(v1) != nullptr);
					},
					conRule::Group, conRule::Output),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				if (v0 != getObject<visualNode>(v1)->getNodeBoard())
					return false;
				group::clear(v2);
				groupIterator gi(v2);
				connectionIterator ci(v1);
				do
				{
					auto vConnection = getObject<visualConnection>(ci.get());
					if ((vConnection != nullptr)&&(vConnection->getNodeBoard() == v0))
					{
						gi.insert(hub::firstEmptyPort(ci.get()));
					}
				} while (ci.stepForward());
				/////////////////////////////////////////////////////недоделано
				return true;
			}}),
				namedOperation(L"getAllConnectedVNodes", operation{
				connectionRule(
					conRule::ExternalObject, conRule::Input, nullptr,
					conRule::ExternalObject, conRule::Input,
					[](nodePtr v1)
					{
						return (getObject<visualNode>(v1) != nullptr);
					},
					conRule::Group, conRule::Output),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				if (v0 != getObject<visualNode>(v1)->getNodeBoard())
					return false;
				group::clear(v2);
				groupIterator gi(v2);
				connectionIterator ci(v1);
				do
				{
					auto vConnection = getObject<visualConnection>(ci.get());
					if ((vConnection != nullptr) && (vConnection->getNodeBoard() == v0))
					{
						gi.insert(hub::firstEmptyPort(vConnection->getOtherEnd(v1)));
					}
				} while (ci.stepForward());
				/////////////////////////////////////////////////////недоделано
				return true;
			}}),
		namedOperation(L"visualized", operation{
				connectionRule(
					conRule::ExternalObject, conRule::Input, nullptr,
					conRule::AnyPointer, conRule::Input, nullptr,
					conRule::I64Variable, conRule::Output, nullptr),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				auto nBoard = v0->getData<nodeBoard*>();
				v2->setData<i64>(nBoard->visualized(v1) != nullptr);
				return true;
			}
			}),*/
	};
}
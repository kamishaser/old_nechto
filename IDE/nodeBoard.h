#pragma once
#include "visualNode.h"
#include "visualConnection.h"
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
	struct nodeBoard : public externalObject
	{
		nodeBoard()//!!!������ � ����, ������ ����� new!!!
			:externalObject(newNode(node::ExternalObject, 1))
		{
			NumNumConnect(get(), newNode(node::Group), 0, 0);
			NumNumConnect(get(), newNode(node::Group), 1, 0);
			NumNumConnect(get(), newNode(node::Group), 2, 0);
		}
		nodePtr vNodeGroup()
		{
			return exObj->connection[0];
		}
		nodePtr vConnectionGroup()
		{
			return exObj->connection[1];
		}
		//����� ��������� ������� ���������� ��� � ����������
		nodePtr taggedGroup()
		{
			return exObj->connection[2];
		}
		
		visualNode* addNode(
			nodePtr emptyExternalObject, nodePtr v1 = nullNodePtr)
		{
			std::cout << "addNode" << std::endl;
			visualNode* vn = new visualNode(
				emptyExternalObject, vNodeGroup(), v1);
			vn->frame.position += randomOffset(200) + glm::vec2(200.f, 400.f);
			return vn;
		}
		visualConnection* addConnection(nodePtr emptyExternalObject,
			visualNode* vn1, visualNode* vn2)
		{
			std::cout << "addConnection" << std::endl;
			visualConnection* vc = 
				new visualConnection(
					emptyExternalObject, vConnectionGroup(), vn1, vn2);
			return vc;
		}
		bool onThisBoard(visualNode* vNode)
		{
			assert(vNode);
			return vNodeGroup() == vNode->get()->connection[3];
		}
		bool onThisBoard(visualConnection* vCon)
		{
			assert(vCon);
			return vConnectionGroup() == vCon->get()->connection[3];
			//��������!!!! ���� ����������
		}
		bool connected(visualNode* vNode1, visualNode* vNode2)
		{
			if (!onThisBoard(vNode1) || !onThisBoard(vNode2))
				return false;
			connectionIterator i1(vNode1->get());
			do
			{
				visualConnection* vscon = visualConnection::getByNode(i1.get());
				if (vscon)
				{
					if (i1.get()->connection[0] == vNode2->get() ||
						i1.get()->connection[1] == vNode2->get())
						return true;
				}
			} while (i1.stepForward());
			return false;
		}
		/*���������� nullptr (���������������� � false) ����
		�� ������ ����� ��� visualNode ����������� �� ��� ����
		//����� ���������� ��������� �� visualNode*/
		visualNode* visualized(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			groupIterator gi(vNodeGroup());
			do
			{
				if(gi.get().exist() && (gi.get()->connection[0] == v1))
					return gi.get()->getData<visualNode*>();
			} while (gi.stepForward());
			return nullptr;
		}
		static nodeBoard* getByNode(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalObject)
				return nullptr;
			return dynamic_cast<nodeBoard*>(v1->getData<externalObject*>());
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
		virtual const conRule& getConnectionRule()const override
		{
			return cRule;
		}

	};
	const std::wstring nodeBoard::typeName = L"nechtoIde.nodeBoard";
	const connectionRule nodeBoard::cRule = connectionRule{
		conRule::Group, conRule::In_Output, nullptr,
		conRule::Group, conRule::In_Output, nullptr,
		conRule::Group, conRule::In_Output, nullptr
	};
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
						return (visualNode::getByNode(v1) != nullptr);
					},
					conRule::Group, conRule::Output),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				if (v0 != visualNode::getByNode(v1)->getNodeBoard())
					return false;
				group::clear(v2);
				groupIterator gi(v2);
				connectionIterator ci(v1);
				do
				{
					auto vConnection = visualConnection::getByNode(ci.get());
					if ((vConnection != nullptr)&&(vConnection->getNodeBoard() == v0))
					{
						gi.insert(hub::firstEmptyPort(ci.get()));
					}
				} while (ci.stepForward());
				/////////////////////////////////////////////////////����������
				return true;
			}}),
				namedOperation(L"getAllConnectedVNodes", operation{
				connectionRule(
					conRule::ExternalObject, conRule::Input, nullptr,
					conRule::ExternalObject, conRule::Input,
					[](nodePtr v1)
					{
						return (visualNode::getByNode(v1) != nullptr);
					},
					conRule::Group, conRule::Output),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				if (v0 != visualNode::getByNode(v1)->getNodeBoard())
					return false;
				group::clear(v2);
				groupIterator gi(v2);
				connectionIterator ci(v1);
				do
				{
					auto vConnection = visualConnection::getByNode(ci.get());
					if ((vConnection != nullptr) && (vConnection->getNodeBoard() == v0))
					{
						gi.insert(hub::firstEmptyPort(vConnection->getOtherEnd(v1)));
					}
				} while (ci.stepForward());
				/////////////////////////////////////////////////////����������
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
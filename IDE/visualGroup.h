#pragma once
#include "externalObject.h"
#include "visualNode.h"
#include "rect.h"
namespace nechto::ide
{
	
	//���������� 0 - �������������� ������ ��� (��� ����� 0)
	//c��������� 3 - vGroupGroup �� nodeBoard
	//����� ������ � �������� (��� ������, ���� �������� �� ���� ����� ������������)
	//������ visualNode
	struct visualGroup :public externalObject
	{
		rect frame;
		visualGroup(nodePtr emptyExternalObject, glm::vec2 startPoint = glm::vec2{0,0})
			:externalObject(emptyExternalObject), frame(startPoint, glm::vec2{1.f, 1.f})
			//��� �������� ����, �������� � ��� ������ !!!������ �������� ����� new!!!
		{
			NumNumConnect(get(), newNode(node::Group), 0, 0);
		}
		nodePtr vNodeGroup() const
		{
			return getConnection(0);
		}
		nodePtr getNodeBoard() const
		{
			nodePtr temp = getConnection(3);
			if (!temp.exist())
				return nullNodePtr;
			return temp->connection[0];
		}
		bool contains(visualNode* vNode) const
		{
			return (vNodeGroup() == vNode->getConnection(0));
		}
		//�������� ������
		virtual void update() {}
		//��������� ���� � ������ ��������� ����
		void addNode(visualNode* vNode) const
		{
			//vNodeGroup ����� ��������� ���� ������ �� ������ nodeBoard
			assert(vNode->getNodeBoard() == getNodeBoard());
			IterIterConnect(connectionIterator(vNode->get(), 1),
				group::firstEmptyPort(vNodeGroup()));
		}
		i64 numberOfVNodes() const
		{
			return group::numberOfMembers(vNodeGroup());
		}
		bool allNodeOnOneNodeBoard() const
		{
			nodePtr nBoardNode = getNodeBoard();
			groupIterator gi(vNodeGroup());
			do
			{
				auto vNode = visualNode::getByNode(gi.get());
				if (vNode)
				{
					if (vNode->getNodeBoard() != nBoardNode)
						return false;
				}
			} while (gi.stepForward());
			return true;
		}

		//��������� ����� ���� ��� ������
		static rect* getRect(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (!typeCompare(v1, node::ExternalObject))
				return nullptr;
			auto vNode = visualNode::getByNode(v1);
			if (vNode)
				return &vNode->frame;
			auto vGroup = visualGroup::getByNode(v1);
			if (vGroup)
				return &vGroup->frame;
			return nullptr;
		}
		
		//////////////////////////////////////////////////////////////
		virtual ~visualGroup()
		{
			if (vNodeGroup().exist())
				deleteNode(vNodeGroup());
		}
		/*��������� ��������� �� visualGroup �� �������.
		���������� nullptr ��� ��������������*/
		static visualGroup* getByNode(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalObject)
				return nullptr;
			return dynamic_cast<visualGroup*>(v1->getData<externalObject*>());
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
	const std::wstring visualGroup::typeName = L"nechtoIde.visualGroup";
	const connectionRule visualGroup::cRule = connectionRule{};
	const staticNodeOperationSet visualGroup::methodSet
	{
		/*namedOperation(L"nothing", operation{
				connectionRule(conRule::ExternalObject, conRule::Input, nullptr),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				return true;
			}})*/
	};
	
}
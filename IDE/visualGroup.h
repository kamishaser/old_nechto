#pragma once
#include "namedExConGroup.h"
#include "visualNode.h"
#include "rect.h"
#include "group.h"
namespace nechto::ide
{

	//���������� 0 - �������������� ������ ��� (��� ����� 0)
	//connection 1 - ������������ ������
	//c��������� 3 - vGroupGroup �� nodeBoard
	//����� ������ � �������� (��� ������, ���� �������� �� ���� ����� ������������)
	//������ visualNode
	struct visualGroup :public namedExConGroup
	{
		rect frame;
		visualGroup(nodePtr emptyExternalObject, const std::wstring& name,
			glm::vec2 startPoint = glm::vec2{ 0,0 })
			:namedExConGroup(emptyExternalObject, name),
			frame(startPoint, glm::vec2{ 1.f, 1.f }) {}
		nodePtr vNodeGroup() const
		{
			return getGroup();
		}
		nodePtr getNodeBoard() const
		{
			nodePtr temp = node().connection(3);
			if (!temp.exist())
				return nullptr;
			return temp.connection(0);
		}
		bool contains(objectPtr<visualNode> vNode) const
		{
			return (vNodeGroup() == vNode.connection(0));
		}
		//�������� ������
		virtual void update() {}
		//��������� ���� � ������ ��������� ����
		void addNode(objectPtr<visualNode> vNode) const
		{
			//vNodeGroup ����� ��������� ���� ������ �� ������ nodeBoard
			assert(vNode->getNodeBoard() == getNodeBoard());
			IterIterConnect(portIterator(vNode, 1),
				firstEmptyGroupPort(getGroup()));
		}
		void addGroup(objectPtr<visualGroup> vGroup) const
		{
			//vNodeGroup ����� ��������� ���� ������ �� ������ nodeBoard
			assert(vGroup->getNodeBoard() == getNodeBoard());
			IterHubConnect(firstEmptyGroupPort(vNodeGroup()), vGroup);
		}
		i64 numberOfVNodes() const
		{
			return groupOperations::numberOfMembers(vNodeGroup());
		}
		bool allNodeOnOneNodeBoard() const
		{
			nodePtr nBoardNode = getNodeBoard();
			groupIterator gi(vNodeGroup());
			do
			{
				auto vNode = getObject<visualNode>(gi.get());
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
			if (objectPtr<visualNode>::match(v1))
				return &objectPtr<visualNode>(v1)->frame;
			if (objectPtr<visualGroup>::match(v1))
				return &objectPtr<visualGroup>(v1)->frame;
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
	};
	const std::wstring visualGroup::typeName = L"nechtoIde.visualGroup";
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
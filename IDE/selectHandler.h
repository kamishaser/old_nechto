#pragma once

#include "mouseHandler.h"

namespace nechto::ide
{
	class selectHandler
	{
	public:
		display& dp;
		namedExCon groupOfSelected;//���������� ����
		namedExCon lastSelectedNode;//4 ��������� ���������� ����

		selectHandler(display& dplay)
			:dp(dplay),
			groupOfSelected(L"groupOfSelected"),
			lastSelectedNode(L"lastSelected")
		{
			NumNumConnect(groupOfSelected.get(), newNode(node::Group), 0, 0);
		}
		~selectHandler()
		{
			deleteNode(selectedGroup());
		}

		void moveAllSelected(glm::vec2 offset)
		{
			groupIterator gi(selectedGroup());
			do
			{
				auto vNode = visualNode::getByNode(gi.get());
				if (vNode)
				{
					vNode->frame.position += offset;
				}
			} while (gi.stepForward());
		}

		nodePtr selectedGroup() const
		{
			return groupOfSelected.getConnection(0);
		}
		bool contains(visualNode* vNode) const
		{
			return group::contains(selectedGroup(), vNode->get());
		}
		bool isLastSelected(visualNode* vNode) const
		{
			return lastSelected() == vNode->get();
		}
		nodePtr lastSelected() const
		{
			return lastSelectedNode.getConnection(0);
		}
		//�������� ���� � ������ ����������. ���������� false, ���� ��� ���������
		bool select(visualNode* vNode)
		{
			if (contains(vNode))
				return false;
			IterHubConnect(group::firstEmptyPort(selectedGroup()), vNode->get());
			NumHubConnect(lastSelectedNode.get(), vNode->get(), 0);
			std::wcout << nodeProperties(vNode->get()) << std::endl;
			return true;
		}
		void deselect(visualNode* vNode)
		{
			disconnect(selectedGroup(), vNode->get());
			if (isLastSelected(vNode))
				connectionIterator(lastSelectedNode.get(), 0).disconnect();
		}
		void deselectAll()
		{
			group::clear(selectedGroup());
		}
	};
}
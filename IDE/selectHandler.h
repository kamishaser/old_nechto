#pragma once

#include "mouseHandler.h"

namespace nechto::ide
{
	class selectHandler
	{
	public:
		namedExCon groupOfSelected;//���������� ����
		namedExCon lastSelectedNode;//4 ��������� ���������� ����

		int numberOfLastSelected = -1;//����� ��������� ���������� ����

		selectHandler()
			:groupOfSelected(L"groupOfSelected"),
			lastSelectedNode(L"lastSelected")
		{
			NumNumConnect(groupOfSelected.get(), newNode(node::Group), 0, 0);
		}
		~selectHandler()
		{
			deleteNode(selectedGroup());
		}

		nodePtr selectedGroup() const
		{
			return groupOfSelected.getConnection(0);
		}
		bool contains(visualNode* vNode) const
		{
			return group::contains(selectedGroup(), vNode->get());
		}
		//�������� ���� � ������ ����������. ���������� false, ���� ��� ���������
		bool select(visualNode* vNode)
		{
			if (contains(vNode))
				return false;
			IterHubConnect(group::firstEmptyPort(selectedGroup()), vNode->get());
			
			if (numberOfLastSelected < 3)
			{
				++numberOfLastSelected;
				NumHubConnect(
					lastSelectedNode.get(), vNode->get(), numberOfLastSelected);
			}
			else
			{
				lsBackShift();
				NumHubConnect(lastSelectedNode.get(), vNode->get(), 3);
			}
			print(L"select");
			return true;
		}
		//����� ���������
		void deselect(visualNode* vNode)
		{
			disconnect(selectedGroup(), vNode->get());
			nodePtr vls = lastSelectedNode.get();
			for(int i = 0; i < 4; ++i)
				if (lastSelectedNode.getConnection(i) == vNode->get())
				{
					nodePtr vls = lastSelectedNode.get();
					numDisconnect(vls, i);
					if (i != 3)
					{
						for (int i1 = i; i1 < 3; ++i1)
							vls->connection[i1] = vls->connection[i1 + 1].load();
						vls->connection[3] = nullNodePtr;
					}
					if (numberOfLastSelected >= i)
						--numberOfLastSelected;
					break;
				}
		}
		//����� ��������� �� ����
		void deselectAll()
		{
			group::clear(selectedGroup());
			for (int i = 0; i < 4; ++i)
				numDisconnect(lastSelectedNode.get(), i);
			numberOfLastSelected = -1;
		}
		void selectGroup(nodePtr group)
		{
			assert(typeCompare(group, node::Group));
			groupIterator gi(group);
			do
			{
				auto vNode = visualNode::getByNode(gi.get());
				if (vNode)
					select(vNode);
			} while (gi.stepForward());
		}
		
		nodePtr lastSelected() const
		{
			return lastSelectedNode.getConnection(numberOfLastSelected);
		}
		bool isLastSelected(visualNode* vNode) const
		{
			return lastSelected() == vNode->get();
		}
		void resetLastSelected()
		{
			for (int i = 0; i < 4; ++i)
				numDisconnect(lastSelectedNode.get(), 0);
		}
		//����� ��������� ���������� �����
		void lsForwardShift()
		{
			nodePtr vls = lastSelectedNode.get();
			nodePtr temp = vls->connection[3];
			for (int i = 2; i >= 0; --i)
				vls->connection[i + 1] = vls->connection[i].load();
			vls->connection[0] = temp;
		}
		//����� ��������� ���������� �����
		void lsBackShift()
		{
			nodePtr vls = lastSelectedNode.get();
			nodePtr temp = vls->connection[0];
			for (int i = 0; i < 3; ++i)
				vls->connection[i] = vls->connection[i + 1].load();
			vls->connection[3] = temp;
		}
	};
}
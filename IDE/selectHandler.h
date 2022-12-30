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
			NumNumConnect(groupOfSelected.node(), creator::createGroup(), 0, 0);
		}
		~selectHandler()
		{
			creator::deleteNode(selectedGroup());
		}

		nodePtr selectedGroup() const
		{
			return groupOfSelected.node().connection(0);
		}
		bool contains(visualNode* vNode) const
		{
			return groupOperations::contains(selectedGroup(), vNode->node());
		}
		//�������� ���� � ������ ����������. ���������� false, ���� ��� ���������
		bool select(visualNode* vNode)
		{
			if (contains(vNode))
				return false;
			IterHubConnect(firstEmptyGroupPort(selectedGroup()), vNode->node());
			
			if (numberOfLastSelected < 3)
			{
				++numberOfLastSelected;
				NumHubConnect(
					lastSelectedNode.node(), vNode->node(), numberOfLastSelected);
			}
			else
			{
				lsBackShift();
				NumHubConnect(lastSelectedNode.node(), vNode->node(), 3);
			}
			std::wcout << L"select" << std::endl;
			return true;
		}
		//����� ���������
		void deselect(visualNode* vNode)
		{
			nearestDisconnect(selectedGroup(), vNode->node());
			nodePtr vls = lastSelectedNode.node();
			for(int i = 0; i < 4; ++i)
				if (lastSelectedNode.node().connection(i) == vNode->node())
				{
					nodePtr vls = lastSelectedNode.node();
					nearestDisconnect(vls, i);
					if (i != 3)
					{
						for (int i1 = i; i1 < 3; ++i1)
							vls.connection(i1) = vls.connection(i1 + 1);
						vls.connection(3) = nullptr;
					}
					if (numberOfLastSelected >= i)
						--numberOfLastSelected;
					break;
				}
		}
		//����� ��������� �� ����
		void deselectAll()
		{
			groupOperations::clear(selectedGroup());
			for (int i = 0; i < 4; ++i)
				nearestDisconnect(lastSelectedNode.node(), i);
			numberOfLastSelected = -1;
		}
		void selectGroup(nodePtr group)
		{
			assert(typeCompare(group, nodeT::Group));
			groupIterator gi(group);
			do
			{
				auto vNode = getObject<visualNode>(gi.get());
				if (vNode)
					select(vNode);
			} while (gi.stepForward());
		}
		
		nodePtr lastSelected() const
		{
			return lastSelectedNode.node().connection(numberOfLastSelected);
		}
		bool isLastSelected(visualNode* vNode) const
		{
			return lastSelected() == vNode->node();
		}
		void resetLastSelected()
		{
			for (int i = 0; i < 4; ++i)
				nearestDisconnect(lastSelectedNode.node(), 0);
		}
		//����� ��������� ���������� �����
		void lsForwardShift()
		{
			nodePtr vls = lastSelectedNode.node();
			nodePtr temp = vls.connection(3);
			for (int i = 2; i >= 0; --i)
				vls.connection(i + 1) = vls.connection(i);
			vls.connection(0) = temp;
		}
		//����� ��������� ���������� �����
		void lsBackShift()
		{
			nodePtr vls = lastSelectedNode.node();
			nodePtr temp = vls.connection(0);
			for (int i = 0; i < 3; ++i)
				vls.connection(i) = vls.connection(i + 1);
			vls.connection(3) = temp;
		}
	};
}
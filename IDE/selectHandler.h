#pragma once

#include "mouseHandler.h"

namespace nechto::ide
{
	class selectHandler
	{
	public:
		namedExCon groupOfSelected;//выделенные ноды
		namedExCon lastSelectedNode;//4 последние выделенные ноды

		int numberOfLastSelected = -1;//номер последней выделенной ноды

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
		//добавить ноду в группу выделенных. Возвращает false, если уже выделенна
		bool select(visualNode* vNode)
		{
			if (contains(vNode))
				return false;
			PointerHubConnect(firstEmptyGroupPort(selectedGroup()), vNode->node());
			
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
		//снять выделение
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
							connecter::swap(portPointer(vls, i1 + 1), portPointer(vls, i1));
					}
					if (numberOfLastSelected >= i)
						--numberOfLastSelected;
					break;
				}
		}
		//снять выделение со всех
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
			groupPointer gi(group);
			do
			{
				auto vNode = getEntity<visualNode>(gi.get());
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
		//сдвиг последних выделенных вперёд
		void lsForwardShift()
		{
			nodePtr vls = lastSelectedNode.node();
			for (int i = 2; i >= 0; --i)
				connecter::swap(portPointer(vls, i + 1), portPointer(vls, i));
		}
		//сдвиг последних выделенных назад
		void lsBackShift()
		{
			nodePtr vls = lastSelectedNode.node();
			for (int i = 0; i < 3; ++i)
				connecter::swap(portPointer(vls, i + 1), portPointer(vls, i));
		}
	};
}
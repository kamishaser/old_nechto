#pragma once
#include "rect.h"
#include "group.h"
#include "ideFactory.h"
#include "ideStructurePack.h"

namespace nechto::ide
{
	struct visualGroup
	{
		const path imp = "0"_np; //проход через посредника
		nodePtr vgNode;
		ideFactory& fact;
		visualGroup(nodePtr node, ideFactory& f)
			:vgNode(node), fact(f)
		{}
		bool isCorrect()
		{
			return
				groupPtr::match(sPack::vGroup::vBlockGroup / vgNode) &&
				groupPtr::match(sPack::vGroup::vConnectionGroup / vgNode) &&
				(sPack::vGroup::nodeBoard / vgNode).exist() &&
				allNodeOnOneNodeBoard();
		}
		groupPtr vNodeGroup() const
		{
			return sPack::vGroup::vBlockGroup / vgNode;
		}
		groupPtr vConnectionGroup() const
		{
			return sPack::vGroup::vBlockGroup / vgNode;
		}
		groupPtr vGroupGroup() const
		{
			return sPack::vGroup::vBlockGroup / vgNode;
		}
		nodePtr getNodeBoard() const
		{
			return sPack::vGroup::nodeBoard / vgNode;
		}
		nodePtr frameNode() const
		{
			return sPack::vGroup::frame / vgNode;
		}
		bool contains(nodePtr vNode) const
		{
			return (sPack::vNode::vGroup / vNode) == vgNode;
		}
		nodePtr addNode() const
		{
			return fact.fabricateVNode(getNodeBoard(), vgNode);
		}
		//nodePtr addGroup(nodePtr vGroup) const
		//{
		//	//vNodeGroup может содержать ноды только из одного nodeBoard
		//	assert(vGroup->getNodeBoard() == getNodeBoard());
		//	IterHubConnect(firstEmptyGroupPort(vNodeGroup()), vGroup);
		//}
		bool allNodeOnOneNodeBoard() const
		{
			nodePtr nBoard = getNodeBoard();
			groupPointer gi(vNodeGroup());
			do
			{
				nodePtr vNode = imp / gi.get();
				if (!vNode.exist())
					continue;
				if ((sPack::vNode::nodeBoard / vNode) != nBoard)
					return false;
			} while (gi.stepForward());
			return true;
		}
		//обновить группу
		virtual bool update() const
		{//true, если требуется обновлять вверх (изменился фрейм группы)
			return true;
		}
		//перемещение всех блоков в группе
		void shift(glm::vec2 offset) const
		{
			groupPointer gi(vNodeGroup());
			do
			{
				if (!gi.get().exist())
					continue;
				"0"_np / sPack::vNode::position / gi.get() += offset;

			} while (gi.stepForward());
		}
	protected:
	};
}
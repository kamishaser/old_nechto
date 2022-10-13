#pragma once
#include "periodLimiter.h"
#include "nodeBoard.h"
#include "display.h"

namespace nechto::ide
{
	class autoExpandHandler
	{
		
	public:
		display& dp;
		autoExpandHandler(display& dplay)
			:dp(dplay) {}
		void update()
		{
			deleteNonExistentNodes();
			deleteNonExistentConnections();
			expand();
		}
	private:
		bool checkAndExpand(hubIterator expandAim, visualNode* vNode1)
		{
			if (!expandAim.get().exist())
				return false;
			//visualNode и visualConnection отображать запрещено
			auto vNode2 = visualNode::getByNode(expandAim.get());
			if (vNode2 != nullptr &&
				dp.workBoard.onThisBoard(vNode2))
				return false;
			auto vConnection =
				visualConnection::getByNode(expandAim.get());
			if (vConnection != nullptr &&
				dp.workBoard.onThisBoard(vConnection))
				return false;
			vNode2 = dp.workBoard.visualized(expandAim.get());
			if (vNode2 != nullptr)
			{//если нода визуализированна, надо проверить наличие соединения с ней 
				if (!dp.workBoard.connected(vNode1, vNode2))
				{//если нету - добавить
					vConnection = new visualConnection(newExObjNode(), vNode1, vNode2);
					dp.workBoard.addConnection(vConnection);
				}
			}
			else
			{
				//если нода не визуализированна, надо добавить visualNode 
				vNode2 = new visualNode(newExObjNode(), expandAim.get());
				dp.workBoard.addNode(vNode2);
				vConnection = new visualConnection(newExObjNode(), vNode1, vNode2); 
				dp.workBoard.addConnection(vConnection);
			}
			return true;
		}
		void deleteNonExistentNodes()
		{
			groupIterator gi(dp.workBoard.vNodeGroup());
			do
			{
				nodePtr v1 = gi.get();
				if (v1.exist())
				{
					auto vNode = visualNode::getByNode(v1);
					if (!v1->connection[0].load().exist() || vNode == nullptr)
					{
						
						deleteNode(v1);
					}
				}
			} while (gi.stepForward());
		}
		void deleteNonExistentConnections()
		{
			groupIterator gi(dp.workBoard.vConnectionGroup());
			do
			{
				auto vConnection = visualConnection::getByNode(gi.get());
				if (!vConnection)
					continue;
				auto vNode0 = visualNode::getByNode(vConnection->getConnection(0));
				auto vNode1 = visualNode::getByNode(vConnection->getConnection(1));
				if (vNode0 == nullptr || vNode1 == nullptr)
				{
					deleteNode(gi.get());
				}
				else
				{
					nodePtr v0 = vNode0->getConnection(0);
					nodePtr v1 = vNode1->getConnection(0);
					if (v0.exist() && v1.exist() && !hasConnection(v0, v1))
						deleteNode(gi.get());
				}
			} while (gi.stepForward());
		}
		void expand()
		{
			groupIterator gi(dp.workBoard.vNodeGroup());
			do
			{
				if (!gi.get().exist() || !gi.get()->connection[0].load().exist())
					continue;//не существующие соединения интереса не представляют
				auto vNode1 = visualNode::getByNode(gi.get());
				nodePtr v1 = gi.get()->connection[0];
				if (typeCompare(v1, node::Group))
				{
					groupIterator gi2(v1);
					do
					{
						checkAndExpand(gi2, vNode1);
					} while (gi2.stepForward());
				}
				connectionIterator ci(v1);
				do
				{
					checkAndExpand(ci, vNode1);
				} while (ci.stepForward());
			} while (gi.stepForward());
		}
	};
}

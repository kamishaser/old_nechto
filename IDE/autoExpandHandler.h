#pragma once
#include "periodLimiter.h"
#include "nodeBoard.h"

namespace nechto::ide
{
	class autoExpandHandler
	{
		bool checkAndExpand(hubIterator expandAim, visualNode* vNode1)
		{
			if (!expandAim.get().exist())
				return false;
			//visualNode и visualConnection отображать запрещено
			auto vNode2 = visualNode::getByNode(expandAim.get());
			if (vNode2 != nullptr &&
				nBoard->onThisBoard(vNode2))
				return false;
			auto vConnection =
				visualConnection::getByNode(expandAim.get());
			if (vConnection != nullptr &&
				nBoard->onThisBoard(vConnection))
				return false;
			vNode2 = nBoard->visualized(expandAim.get());
			if (vNode2 != nullptr)
			{//если нода визуализированна, надо проверить наличие соединения с ней 
				if (!nBoard->connected(vNode1, vNode2))
				{//если нету - добавить
					vConnection = nBoard->addConnection(
						newNode(node::ExternalObject, 1),
						vNode1, vNode2);
				}
			}
			else
			{
				//если нода не визуализированна, надо добавить visualNode 
				vNode2 = nBoard->addNode(
					newNode(node::ExternalObject, 1), expandAim.get());
				vConnection = nBoard->addConnection(
					newNode(node::ExternalObject, 1),
					vNode1, vNode2);
			}
			return true;
		}
	public:
		nodeBoard* nBoard;
		autoExpandHandler(nodeBoard* nbn)
			:nBoard(nbn) {}
		void update()
		{
			deleteNonExistentNodes();
			deleteNonExistentConnections();
			expand();
		}
		void deleteNonExistentNodes()
		{
			groupIterator gi(nBoard->vNodeGroup());
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
			groupIterator gi(nBoard->vConnectionGroup());
			do
			{
				if (!gi.get().exist())
					continue;
				auto vConnection = visualConnection::getByNode(gi.get());
				auto vNode0 = visualNode::getByNode(gi.get()->connection[0]);
				auto vNode1 = visualNode::getByNode(gi.get()->connection[1]);
				if (!(vConnection && vNode0 && vNode1))
				{
					deleteNode(gi.get());
					return;
				}
				nodePtr v0 = vNode0->getConnection(0);
				nodePtr v1 = vNode1->getConnection(0);
				if (!(v0.exist() && v1.exist() &&
					(vConnection != nullptr) && hasConnection(v0, v1)))
					deleteNode(gi.get());
			} while (gi.stepForward());
		}
		void expand()
		{
			groupIterator gi(nBoard->vNodeGroup());
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

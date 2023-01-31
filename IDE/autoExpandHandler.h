#pragma once
#include "periodLimiter.h"
#include "nodeBoard.h"
#include "GUI.h"

namespace nechto::ide
{
	class autoExpandHandler
	{
		
	public:
		GUI& gui;
		autoExpandHandler(GUI& g)
			:gui(g) {}
		void update()
		{
			deleteNonExistentNodes();
			deleteNonExistentConnections();
			expand();
		}
	private:
		bool checkAndExpand(pointer expandAim, visualNode* vNode1)
		{
			if (!expandAim.get().exist())
				return false;
			//visualNode и visualConnection отображать запрещено
			auto vNode2 = getEntity<visualNode>(expandAim.get());
			if (vNode2 != nullptr &&
				gui.workBoard.onThisBoard(getEntityPtr(vNode2)))
				return false;
			auto vConnection =
				getEntity<visualConnection>(expandAim.get());
			if (vConnection != nullptr &&
				gui.workBoard.onThisBoard(entityPtr<visualConnection>(expandAim.get())))
				return false;
			vNode2 = getEntity<visualNode>(gui.workBoard.visualized(expandAim.get()));
			if (vNode2 != nullptr)
			{//если нода визуализированна, надо проверить наличие соединения с ней 
				if (!gui.workBoard.connected(getEntityPtr(vNode1), getEntityPtr(vNode2)))
				{//если нету - добавить
					gui.workBoard.addConnection(visualConnection::create(getEntityPtr(vNode1), getEntityPtr(vNode2)));
				}
			}
			else
			{
				//если нода не визуализированна, надо добавить visualNode 
				vNode2 = new visualNode(creator::createEntity(1), expandAim.get());
				gui.workBoard.addNode(getEntityPtr(vNode2));
				gui.workBoard.addConnection(visualConnection::create(getEntityPtr(vNode1), getEntityPtr(vNode2)));
			}
			return true;
		}
		void deleteNonExistentNodes()
		{
			groupPointer gi(gui.workBoard.vNodeGroup());
			do
			{
				nodePtr v1 = gi.get();
				if (v1.exist())
				{
					auto vNode = getEntity<visualNode>(v1);
					if (!v1.connection(0).exist() || vNode == nullptr)
					{
						
						creator::deleteNode(v1);
					}
				}
			} while (gi.stepForward());
		}
		void deleteNonExistentConnections()
		{
			groupPointer gi(gui.workBoard.vConnectionGroup());
			do
			{
				auto vConnection = getEntity<visualConnection>(gi.get());
				if (!vConnection)
					continue;
				auto vNode0 = getEntity<visualNode>(gi.get().connection(0));
				auto vNode1 = getEntity<visualNode>(gi.get().connection(1));
				if (vNode0 == nullptr || vNode1 == nullptr)
				{
					creator::deleteNode(gi.get());
				}
				else
				{
					nodePtr v0 = vNode0->node().connection(0);
					nodePtr v1 = vNode1->node().connection(0);
					if (v0.exist() && v1.exist() && !hasConnection(v0, v1))
						creator::deleteNode(gi.get());
				}
			} while (gi.stepForward());
		}
		void expand()
		{
			groupPointer gi(gui.workBoard.vNodeGroup());
			do
			{
				if (!gi.get().exist() || !gi.get().connection(0).exist())
					continue;//не существующие соединения интереса не представляют
				auto vNode1 = getEntity<visualNode>(gi.get());
				nodePtr v1 = gi.get().connection(0);
				if (groupPtr::match(v1))
				{
					groupPointer gi2(v1);
					do
					{
						checkAndExpand(gi2, vNode1);
					} while (gi2.stepForward());
				}
				portPointer ci(v1);
				do
				{
					checkAndExpand(ci, vNode1);
				} while (ci.stepForward());
			} while (gi.stepForward());
		}
	};
}

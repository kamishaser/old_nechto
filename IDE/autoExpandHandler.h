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
		bool checkAndExpand(iterator expandAim, visualNode* vNode1)
		{
			if (!expandAim.get().exist())
				return false;
			//visualNode � visualConnection ���������� ���������
			auto vNode2 = getObject<visualNode>(expandAim.get());
			if (vNode2 != nullptr &&
				gui.workBoard.onThisBoard(getObjectPtr(vNode2)))
				return false;
			auto vConnection =
				getObject<visualConnection>(expandAim.get());
			if (vConnection != nullptr &&
				gui.workBoard.onThisBoard(objectPtr<visualConnection>(expandAim.get())))
				return false;
			vNode2 = getObject<visualNode>(gui.workBoard.visualized(expandAim.get()));
			if (vNode2 != nullptr)
			{//���� ���� ����������������, ���� ��������� ������� ���������� � ��� 
				if (!gui.workBoard.connected(getObjectPtr(vNode1), getObjectPtr(vNode2)))
				{//���� ���� - ��������
					gui.workBoard.addConnection(visualConnection::create(getObjectPtr(vNode1), getObjectPtr(vNode2)));
				}
			}
			else
			{
				//���� ���� �� ����������������, ���� �������� visualNode 
				vNode2 = new visualNode(creator::createObject(1), expandAim.get());
				gui.workBoard.addNode(getObjectPtr(vNode2));
				gui.workBoard.addConnection(visualConnection::create(getObjectPtr(vNode1), getObjectPtr(vNode2)));
			}
			return true;
		}
		void deleteNonExistentNodes()
		{
			groupIterator gi(gui.workBoard.vNodeGroup());
			do
			{
				nodePtr v1 = gi.get();
				if (v1.exist())
				{
					auto vNode = getObject<visualNode>(v1);
					if (!v1.connection(0).exist() || vNode == nullptr)
					{
						
						creator::deleteNode(v1);
					}
				}
			} while (gi.stepForward());
		}
		void deleteNonExistentConnections()
		{
			groupIterator gi(gui.workBoard.vConnectionGroup());
			do
			{
				auto vConnection = getObject<visualConnection>(gi.get());
				if (!vConnection)
					continue;
				auto vNode0 = getObject<visualNode>(gi.get().connection(0));
				auto vNode1 = getObject<visualNode>(gi.get().connection(1));
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
			groupIterator gi(gui.workBoard.vNodeGroup());
			do
			{
				if (!gi.get().exist() || !gi.get().connection(0).exist())
					continue;//�� ������������ ���������� �������� �� ������������
				auto vNode1 = getObject<visualNode>(gi.get());
				nodePtr v1 = gi.get().connection(0);
				if (groupPtr::match(v1))
				{
					groupIterator gi2(v1);
					do
					{
						checkAndExpand(gi2, vNode1);
					} while (gi2.stepForward());
				}
				portIterator ci(v1);
				do
				{
					checkAndExpand(ci, vNode1);
				} while (ci.stepForward());
			} while (gi.stepForward());
		}
	};
}

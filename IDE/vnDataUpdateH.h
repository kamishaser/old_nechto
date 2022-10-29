#pragma once
#include "visualNodeFill.h"
#include "display.h"

namespace nechto::ide
{
	class vnDataUpdateH
	{
	public:
		GUI& gui;
		vnDataUpdateH(GUI& g)
			:gui(g) {}
		void update()
		{
			updateOneBoard(&gui.workBoard, true);
			updateOneBoard(&gui.interfaceBoard, false);
		}
	private:
		void updateOneBoard(nodeBoard* nBoard, bool updateText)
		{
			updateNodes(nBoard, updateText);
			updateConnections(nBoard);
			updateGroups(nBoard);
		}
		void updateNodes(nodeBoard* nBoard, bool updateText)
		{
			groupIterator i1(nBoard->vNodeGroup());
			do
			{
				visualNode* vNode = visualNode::getByNode(i1.get());
				if (vNode)
				{
					if (i1.get()->connection[0].load().exist() && updateText)
					{
						vnText::fill(vNode);
						vnShape::fill(vNode);
					}
					vnLightColor::fill(vNode);
				}
			} while (i1.stepForward());
			
		}
		void updateConnections(nodeBoard* nBoard)
		{
			groupIterator i1(nBoard->vConnectionGroup());
			do
			{
				auto vConnection = visualConnection::getByNode(i1.get());
				if (vConnection)
				{
					auto vNode0 = visualNode::getByNode(
						vConnection->getConnection(0));
					auto vNode1 = visualNode::getByNode(
						vConnection->getConnection(1));
					assert((vNode0) && (vNode1));
					nodePtr v0 = vNode0->getConnection(0);
					nodePtr v1 = vNode1->getConnection(0);
					if (v0.exist() && v1.exist())
					{
						setConType(vConnection, 0, getConType(v0, v1));
						setConType(vConnection, 1, getConType(v1, v0));
					}
				}
			} while (i1.stepForward());
		}
		void updateGroups(nodeBoard* nBoard)
		{
			groupIterator i1(nBoard->vGroupGroup());
			do
			{
				auto vGroup = visualGroup::getByNode(i1.get());
				if (vGroup)
					vGroup->update();
			} while (i1.stepForward());
		}
		void setConType(visualConnection* vConnection, int number, conType cType)
		{
			std::wstring& text = (number) ? vConnection->sText : vConnection->fText;
			switch (cType)
			{
			case nechto::conType::Hub:
				text = L"";
				break;
			case nechto::conType::Group:
				text = L"G";
				break;
			case nechto::conType::N0:
				text = L"N0";
				break;
			case nechto::conType::N1:
				text = L"N1";
				break;
			case nechto::conType::N2:
				text = L"N2";
				break;
			case nechto::conType::N3:
				text = L"N3";
				break;
			default:
				break;
			}
		}
		conType getConType(nodePtr v1, nodePtr v2)
		{
			for (int i = 0; i < 4; ++i)
				if (v1->connection[i] == v2)
					return static_cast<conType>(
						static_cast<int>(conType::N0) + i);
			if (typeCompare(v1, node::Group))
			{
				groupIterator gi(v1);
				do
				{
					if (gi.get() == v2)
						return conType::Group;
				} while (gi.stepForward());
			}
			return conType::Hub;
		}
	};
}
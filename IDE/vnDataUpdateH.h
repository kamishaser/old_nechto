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
				visualNode* vNode = getObject<visualNode>(i1.get());
				if (vNode)
				{
					if (i1.get().connection(0).exist() && updateText)
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
				auto vConnection = getObject<visualConnection>(i1.get());
				if (vConnection)
				{
					auto vNode0 = getObject<visualNode>(i1.get().connection(0));
					auto vNode1 = getObject<visualNode>(i1.get().connection(1));
					assert((vNode0) && (vNode1));
					nodePtr v0 = vNode0->node().connection(0);
					nodePtr v1 = vNode1->node().connection(0);
					if (v0.exist() && v1.exist())
					{
						iterator iter0 = findNearestConnection(v0, v1);
						iterator iter1 = findNearestConnection(v1, v0);
						if (iter0.exist() && iter1.exist())
						{
							setConType(vConnection, 0, 
								iter0.getGlobalPos(), iter0.inGroup());
							setConType(vConnection, 1,
								iter1.getGlobalPos(), iter1.inGroup());
						}
					}
				}
			} while (i1.stepForward());
		}
		void updateGroups(nodeBoard* nBoard)
		{
			groupIterator i1(nBoard->vGroupGroup());
			do
			{
				auto vGroup = getObject<visualGroup>(i1.get());
				if (vGroup)
					vGroup->update();
			} while (i1.stepForward());
		}
		void setConType(visualConnection* vConnection, int sideNumber, 
			int portNumber, bool groupPort = false)
		{
			std::wstring& text = (sideNumber) ? vConnection->sText : vConnection->fText;
			if (groupPort)
				text = std::wstring(L"G") + std::to_wstring(portNumber);
			else if (portNumber < 4)
				text = std::wstring(L"N") + std::to_wstring(portNumber);
			else
				text = L"";//std::wstring(L"H") + std::to_wstring(portNumber);
		}
	};
}
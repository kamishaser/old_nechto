#pragma once
//#include "visualNodeFill.h"
//#include "display.h"
//
//namespace nechto::ide
//{
//	class vnDataUpdateH
//	{
//	public:
//		GUI& gui;
//		vnDataUpdateH(GUI& g)
//			:gui(g) {}
//		void update()
//		{
//			updateOneBoard(&gui.workBoard, true);
//			updateOneBoard(&gui.interfaceBoard, false);
//		}
//	private:
//		void updateOneBoard(nodeBoard* nBoard, bool updateText)
//		{
//			updateNodes(nBoard, updateText);
//			updateConnections(nBoard);
//			updateGroups(nBoard);
//		}
//		void updateNodes(nodeBoard* nBoard, bool updateText)
//		{
//			groupPointer i1(nBoard->vNodeGroup());
//			do
//			{
//				visualNode* vNode = getEntity<visualNode>(i1.get());
//				if (vNode)
//				{
//					if (i1.get().connection(0).exist() && updateText)
//					{
//						vnText::fill(vNode);
//						vnShape::fill(vNode);
//					}
//					vnLightColor::fill(vNode);
//				}
//			} while (i1.stepForward());
//			
//		}
//		void updateConnections(nodeBoard* nBoard)
//		{
//			groupPointer i1(nBoard->vConnectionGroup());
//			do
//			{
//				auto vConnection = getEntity<visualConnection>(i1.get());
//				if (vConnection)
//				{
//					auto vNode0 = getEntity<visualNode>(i1.get().connection(0));
//					auto vNode1 = getEntity<visualNode>(i1.get().connection(1));
//					assert((vNode0) && (vNode1));
//					nodePtr v0 = vNode0->node().connection(0);
//					nodePtr v1 = vNode1->node().connection(0);
//					if (v0.exist() && v1.exist())
//					{
//						pointer ptr0 = findNearestConnection(v0, v1);
//						pointer ptr1 = findNearestConnection(v1, v0);
//						if (ptr0.exist() && ptr1.exist())
//						{
//							setConType(vConnection, 0, 
//								ptr0.getGlobalPos(), ptr0.inGroup());
//							setConType(vConnection, 1,
//								ptr1.getGlobalPos(), ptr1.inGroup());
//						}
//					}
//				}
//			} while (i1.stepForward());
//		}
//		void updateGroups(nodeBoard* nBoard)
//		{
//			groupPointer i1(nBoard->vGroupGroup());
//			do
//			{
//				auto vGroup = getEntity<visualGroup>(i1.get());
//				if (vGroup)
//					vGroup->update();
//			} while (i1.stepForward());
//		}
//		void setConType(visualConnection* vConnection, int sideNumber, 
//			int portNumber, bool groupPort = false)
//		{
//			std::wstring& text = (sideNumber) ? vConnection->sText : vConnection->fText;
//			if (groupPort)
//				text = std::wstring(L"G") + std::to_wstring(portNumber);
//			else if (portNumber < 4)
//				text = std::wstring(L"N") + std::to_wstring(portNumber);
//			else
//				text = L"";//std::wstring(L"H") + std::to_wstring(portNumber);
//		}
//	};
//}
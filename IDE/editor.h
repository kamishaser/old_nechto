#pragma once
//#include "GUI.h"
//#include "button.h"
//#include "buttonList.h"
//#include "singleConnectedEntityGroup.h"
//#include "consistentGroup.h"
//#include "selectHandler.h"
//
//namespace nechto::ide
//{
//	class editor
//	{
//		
//		GUI& gui;
//		selectHandler& sh;
//		//№0 bList
//		unsigned char creatingType = nodeT::Text;
//		unsigned char creatingSubtype = 0;
//
//		unsigned char creatingMathOperationSubtype = 0;
//		
//		conType con1Type = conType::Hub;
//		conType con2Type = conType::Hub;
//	public:
//		buttonList nList{ creator::createEntity(1), 
//			new consistentGroup(creator::createEntity(1),
//				L"тип создаваемой ноды", glm::vec2(5.f, 10.f)),
//				L"тип создаваемой ноды", {
//				L"Text",
//				L"I64",
//				L"F64",
//				L"Group",
//				L"If",
//				L"MathOperation"}, L"Text" };
//		buttonList c1List{ creator::createEntity(1),
//			new consistentGroup(creator::createEntity(1),
//				L"тип создаваемого соединения 1", glm::vec2(5.f, 40.f)),
//			L"тип создаваемого соединения 1", {
//				L"Hub",
//				L"Group",
//				L"N0",
//				L"N1",
//				L"N2",
//				L"N3"}, L"Hub"};
//		buttonList c2List{ creator::createEntity(1),
//			new consistentGroup(creator::createEntity(1),
//				L"тип создаваемого соединения 2", glm::vec2(5.f, 70.f)),
//			L"тип создаваемого соединения 2", {
//				L"Hub",
//				L"Group",
//				L"N0",
//				L"N1",
//				L"N2",
//				L"N3"}, L"Hub" };
//
//		buttonList MathOperationList{ creator::createEntity(1),
//			new consistentGroup(creator::createEntity(1),
//				L"математический оператор", glm::vec2(10.f, 100.f)),
//			L"математический оператор", 
//			typeName::getTypeNameList(typeName::getMathOperationShortSubtypeName, 
//				MathOperationT::Decrement+1) };
//		consistentGroup editorGroup{ creator::createEntity(1), L"редактор", glm::vec2(10, 10) };
//		editor(GUI& g, selectHandler& shandler)
//			:gui(g), sh(shandler)
//		{
//			gui.setbList(nList, [&](buttonList* bList)
//				{
//					auto vNode = getEntity<visualNode>(
//						bList->lClicked()->node().connection(0));
//
//					const std::wstring& name = bList->lClicked()->name;
//					if (name == L"MathOperation")
//					{
//						setActiveTypeAndSubtype(nodeT::MathOperation, 
//							creatingMathOperationSubtype);
//						assert(creatingType == nodeT::MathOperation);
//						if(!MathOperationList.visible())
//							MathOperationList.show(&gui.interfaceBoard);
//						auto vGroup = getEntity<visualGroup>(MathOperationList.getGroup());
//						if (vGroup)
//						{
//							vGroup->frame.size = vNode->frame.size + glm::vec2(0.f, 30.f);
//						}
//					}
//					else
//					{
//						gui.hideAllActiveDropLists();
//						if (name == L"Text")
//							setActiveTypeAndSubtype(nodeT::Text);
//						else if (name == L"I64")
//							setActiveTypeAndSubtype(nodeT::Variable, 1);
//						else if (name == L"F64")
//							setActiveTypeAndSubtype(nodeT::Variable, 0);
//						else if (name == L"If")
//							setActiveTypeAndSubtype(nodeT::Condition);
//						else if (name == L"Group")
//							setActiveTypeAndSubtype(nodeT::Group);
//					}
//				});
//			gui.setbList(c2List, [&](buttonList* bList)
//			{
//					const std::wstring& name = bList->lClicked()->name;
//					if (name == L"Hub")
//						con2Type = conType::Hub;
//					else if (name == L"Group")
//						con2Type = conType::Group;
//					else if (name == L"N0")
//						con2Type = conType::N0;
//					else if (name == L"N1")
//						con2Type = conType::N1;
//					else if (name == L"N2")
//						con2Type = conType::N2;
//					else if (name == L"N3")
//						con2Type = conType::N3;
//			});
//			gui.setbList(c1List, [&](buttonList* bList)
//				{
//					const std::wstring& name = bList->lClicked()->name;
//					if (name == L"Hub")
//						con1Type = conType::Hub;
//					else if (name == L"Group")
//						con1Type = conType::Group;
//					else if (name == L"N0")
//						con1Type = conType::N0;
//					else if (name == L"N1")
//						con1Type = conType::N1;
//					else if (name == L"N2")
//						con1Type = conType::N2;
//					else if (name == L"N3")
//						con1Type = conType::N3;
//				});
//
//			gui.setDropList(MathOperationList, [&](buttonList* bList)
//				{
//					const std::wstring& name = bList->lClicked()->name;
//					for (int i = 0; i < (MathOperationT::Decrement + 1); ++i)
//						if (name == typeName::getMathOperationShortSubtypeName(i))
//						{
//							creatingSubtype = i; 
//							creatingMathOperationSubtype = i;
//						}
//				});
//			gui.interfaceBoard.addGroup(editorGroup.node());
//			editorGroup.addGroup(getEntityPtr(nList.vNodeGroup()));
//			editorGroup.addGroup(getEntityPtr(c1List.vNodeGroup()));
//			editorGroup.addGroup(getEntityPtr(c2List.vNodeGroup()));
//			gui.topGroup.addGroup(editorGroup.node());
//			editorGroup.mode.horisontal = false;
//			editorGroup.update();
//		}
//		
//		visualNode* addNode()
//		{
//			nodePtr v1 = creator::createNode(creatingType, creatingSubtype);
//			auto vNode1 = new visualNode(creator::createEntity(1), v1);
//			gui.workBoard.addNode(getEntityPtr<visualNode>(vNode1));
//			eConnect(vNode1);
//			return vNode1;
//		}
//		void eConnect(visualNode* vNode1)
//		{
//			groupPointer gi(sh.selectedGroup());
//			do
//			{
//				auto vNode2 = getEntity<visualNode>(gi.get());
//				if ((!(vNode2)) || (vNode1->node() == vNode2->node()))
//					continue;
//				eConnect(vNode1, vNode2, con1Type, con2Type);
//			} while (gi.stepForward());
//		}
//		static void eConnect(
//			visualNode* vNode1, visualNode* vNode2, conType ct1, conType ct2)
//		{
//			assert((vNode1) && (vNode2));
//			nodePtr v1 = vNode1->node().connection(0);
//			nodePtr v2 = vNode2->node().connection(0);
//			if (!(v1.exist() && v2.exist()))
//				return;
//			PointerPointerConnect(getPointer(v1, ct1), getPointer(v2, ct2));
//		}
//		static pointer getPointer(nodePtr v1, conType c1)
//		{
//			if (c1 == conType::Hub)
//				return firstEmptyHubPort(v1);
//			if (c1 == conType::Group)
//			{
//				if (typeCompare(v1, nodeT::Group))
//					return firstEmptyGroupPort(v1);
//				else
//					return firstEmptyHubPort(v1);
//			}
//			return portPointer(v1, 
//				static_cast<int>(c1) - static_cast<int>(conType::N0));
//		}
//		
//		void update()
//		{
//		}
//		private:
//		
//		void setActiveTypeAndSubtype(unsigned char type, unsigned char subtype = 0)
//		{
//			creatingType = type;
//			creatingSubtype = subtype;
//		}
//		void hideAllActiveDropLists()
//		{
//			MathOperationList.hide();
//		}
//	};	
//}
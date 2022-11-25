#pragma once
#include "GUI.h"
#include "button.h"
#include "buttonList.h"
#include "namedExConGroup.h"
#include "consistentGroup.h"
#include "selectHandler.h"

namespace nechto::ide
{
	class editor
	{
		GUI& gui;
		selectHandler& sh;
		namedExCon eCon{ u"editor" };
		//№0 bList
		char creatingType = node::Text;
		char creatingSubtype = 0;

		char creatingMathOperatorSubtype = 0;
		
		conType con1Type = conType::Hub;
		conType con2Type = conType::Hub;
	public:
		buttonList nList{ newExObjNode(), 
			new consistentGroup(newExObjNode(),
				u"тип создаваемой ноды", glm::vec2(100.f, 10.f)),
			u"тип создаваемой ноды", true, {
				u"Text",
				u"I64",
				u"F64",
				u"Group",
				u"If",
				u"MathOperator"}, u"Text" };
		buttonList c1List{ newExObjNode(),
			new consistentGroup(newExObjNode(),
				u"тип создаваемого соединения 1", glm::vec2(100.f, 30.f)),
			u"тип создаваемого соединения 1", true, {
				u"Hub",
				u"Group",
				u"N0",
				u"N1",
				u"N2",
				u"N3"}, u"Hub"};
		buttonList c2List{ newExObjNode(),
			new consistentGroup(newExObjNode(),
				u"тип создаваемого соединения 2", glm::vec2(100.f, 30.f)),
			u"тип создаваемого соединения 2", true, {
				u"Hub",
				u"Group",
				u"N0",
				u"N1",
				u"N2",
				u"N3"}, u"Hub" };

		buttonList mathOperatorList{ newExObjNode(),
			new consistentGroup(newExObjNode(),
				u"математический оператор", glm::vec2(100.f, 30.f)),
			u"математический оператор", true, typeName::mathOperatorShortT };
		editor(GUI& g, selectHandler& shandler)
			:gui(g), sh(shandler)
		{
			setbList(nList, [&](buttonList* bList)
				{
					auto vNode = visualNode::getByNode(
						bList->lClicked()->getConnection(0));

					const std::u16string& name = bList->lClicked()->name;
					if (name == u"Text")
						setActiveTypeAndSubtype(node::Text);
					else if (name == u"I64")
						setActiveTypeAndSubtype(node::Variable, 1);
					else if (name == u"F64")
						setActiveTypeAndSubtype(node::Variable, 0);
					else if (name == u"If")
						setActiveTypeAndSubtype(node::ConditionalBranching);
					else if (name == u"Group")
						setActiveTypeAndSubtype(node::Group);
					else if (name == u"MathOperator")
					{
						setActiveTypeAndSubtype(node::MathOperator, 
							creatingMathOperatorSubtype);
						mathOperatorList.show(&gui.interfaceBoard);
						auto vGroup = visualGroup::getByNode(mathOperatorList.getGroup());
						if (vGroup)
						{
							vGroup->frame.size = vNode->frame.size + glm::vec2(0.f, 30.f);
						}
					}
				});
			setbList(c2List, [&](buttonList* bList)
			{
					const std::u16string& name = bList->lClicked()->name;
					if (name == u"Hub")
						con2Type = conType::Hub;
					else if (name == u"Group")
						con2Type = conType::Group;
					else if (name == u"N0")
						con2Type = conType::N0;
					else if (name == u"N1")
						con2Type = conType::N1;
					else if (name == u"N2")
						con2Type = conType::N2;
					else if (name == u"N3")
						con2Type = conType::N3;
			});
			setbList(c1List, [&](buttonList* bList)
				{
					const std::u16string& name = bList->lClicked()->name;
					if (name == u"Hub")
						con1Type = conType::Hub;
					else if (name == u"Group")
						con1Type = conType::Group;
					else if (name == u"N0")
						con1Type = conType::N0;
					else if (name == u"N1")
						con1Type = conType::N1;
					else if (name == u"N2")
						con1Type = conType::N2;
					else if (name == u"N3")
						con1Type = conType::N3;
				});

			setDropList(mathOperatorList, [&](buttonList* bList)
				{
					const std::u16string& name = bList->lClicked()->name;
					for (int i = 0; i < typeName::mathOperatorShortT.size(); ++i)
						if (name == typeName::mathOperatorShortT[i])
						{
							creatingSubtype = i; 
							creatingMathOperatorSubtype = i;
						}
				});
		}
		
		visualNode* addNode()
		{
			nodePtr v1 = newNode(creatingType, creatingSubtype);
			auto vNode1 = new visualNode(newExObjNode(), v1);
			gui.workBoard.addNode(vNode1);
			connect(vNode1);
			return vNode1;
		}
		void connect(visualNode* vNode1)
		{
			groupIterator gi(sh.selectedGroup());
			do
			{
				auto vNode2 = visualNode::getByNode(gi.get());
				if ((!(vNode2)) || (vNode1->get() == vNode2->get()))
					continue;
				connect(vNode1, vNode2, con1Type, con2Type);
			} while (gi.stepForward());
		}
		static void connect(
			visualNode* vNode1, visualNode* vNode2, conType ct1, conType ct2)
		{
			assert((vNode1) && (vNode2));
			nodePtr v1 = vNode1->getConnection(0);
			nodePtr v2 = vNode2->getConnection(0);
			if (!(v1.exist() && v2.exist()))
				return;
			IterIterConnect(getIter(v1, ct1), getIter(v2, ct2));
		}
		static hubIterator getIter(nodePtr v1, conType c1)
		{
			if (c1 == conType::Hub)
				return hub::firstEmptyPort(v1);
			if (c1 == conType::Group)
			{
				if (typeCompare(v1, node::Group))
					return group::firstEmptyPort(v1);
				else
					return hub::firstEmptyPort(v1);
			}
			return connectionIterator(v1, 
				static_cast<int>(c1) - static_cast<int>(conType::N0));
		}
		
		void update()
		{

		}
		private:
		void setbList(buttonList& list, 
			buttonList::clickEventT clickEvent = nullptr)
		{
			auto vGroup = list.vNodeGroup();

			gui.interfaceBoard.addGroup(vGroup);
			gui.topGroup.addGroup(vGroup);
			auto cGroup = dynamic_cast<consistentGroup*>(vGroup);
			if (cGroup)
				cGroup->mode.horisontal = true;
			NumHubConnect(list.get(), eCon.get(), 3);
			list.clickEvent = clickEvent;
			list.show(&gui.interfaceBoard);
		}
		void setDropList(buttonList& list,
			buttonList::clickEventT clickEvent = nullptr)
		{
			auto vGroup = list.vNodeGroup();
			gui.interfaceBoard.addGroup(vGroup);
			auto cGroup = dynamic_cast<consistentGroup*>(vGroup);
			if (cGroup)
			{
				cGroup->distance = 5;
			}
			NumHubConnect(list.get(), eCon.get(), 3);
			list.clickEvent = [clickEvent](buttonList* list)
			{
				clickEvent(list);
				list->hide();
			};
		}
		void setActiveTypeAndSubtype(char type, char subtype = 0)
		{
			creatingType = type;
			creatingSubtype = subtype;
		}
		void hideActiveDropList()
		{}
	};	
}
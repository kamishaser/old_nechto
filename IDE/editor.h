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
		//№0 bList
		char creatingType = nodeT::Text;
		char creatingSubtype = 0;

		char creatingMathOperatorSubtype = 0;
		
		conType con1Type = conType::Hub;
		conType con2Type = conType::Hub;
	public:
		buttonList nList{ creator::createObject(1), 
			new consistentGroup(creator::createObject(1),
				L"тип создаваемой ноды", glm::vec2(100.f, 10.f)),
				L"тип создаваемой ноды", {
				L"Text",
				L"I64",
				L"F64",
				L"Group",
				L"If",
				L"MathOperator"}, L"Text" };
		buttonList c1List{ creator::createObject(1),
			new consistentGroup(creator::createObject(1),
				L"тип создаваемого соединения 1", glm::vec2(100.f, 30.f)),
			L"тип создаваемого соединения 1", {
				L"Hub",
				L"Group",
				L"N0",
				L"N1",
				L"N2",
				L"N3"}, L"Hub"};
		buttonList c2List{ creator::createObject(1),
			new consistentGroup(creator::createObject(1),
				L"тип создаваемого соединения 2", glm::vec2(100.f, 30.f)),
			L"тип создаваемого соединения 2", {
				L"Hub",
				L"Group",
				L"N0",
				L"N1",
				L"N2",
				L"N3"}, L"Hub" };

		buttonList mathOperatorList{ creator::createObject(1),
			new consistentGroup(creator::createObject(1),
				L"математический оператор", glm::vec2(100.f, 60.f)),
			L"математический оператор", 
			typeName::getTypeNameList(typeName::getMathOperatorShortSubtypeName, 
				mathOperatorT::Decrement+1) };
		editor(GUI& g, selectHandler& shandler)
			:gui(g), sh(shandler)
		{
			gui.setbList(nList, [&](buttonList* bList)
				{
					auto vNode = getObject<visualNode>(
						bList->lClicked()->node().connection(0));

					const std::wstring& name = bList->lClicked()->name;
					if (name == L"MathOperator")
					{
						setActiveTypeAndSubtype(nodeT::MathOperator, 
							creatingMathOperatorSubtype);
						if(!mathOperatorList.visible())
							mathOperatorList.show(&gui.interfaceBoard);
						auto vGroup = getObject<visualGroup>(mathOperatorList.getGroup());
						if (vGroup)
						{
							vGroup->frame.size = vNode->frame.size + glm::vec2(0.f, 30.f);
						}
					}
					else
					{
						gui.hideAllActiveDropLists();
						if (name == L"Text")
							setActiveTypeAndSubtype(nodeT::Text);
						else if (name == L"I64")
							setActiveTypeAndSubtype(nodeT::Variable, 1);
						else if (name == L"F64")
							setActiveTypeAndSubtype(nodeT::Variable, 0);
						else if (name == L"If")
							setActiveTypeAndSubtype(nodeT::Condition);
						else if (name == L"Group")
							setActiveTypeAndSubtype(nodeT::Group);
					}
				});
			gui.setbList(c2List, [&](buttonList* bList)
			{
					const std::wstring& name = bList->lClicked()->name;
					if (name == L"Hub")
						con2Type = conType::Hub;
					else if (name == L"Group")
						con2Type = conType::Group;
					else if (name == L"N0")
						con2Type = conType::N0;
					else if (name == L"N1")
						con2Type = conType::N1;
					else if (name == L"N2")
						con2Type = conType::N2;
					else if (name == L"N3")
						con2Type = conType::N3;
			});
			gui.setbList(c1List, [&](buttonList* bList)
				{
					const std::wstring& name = bList->lClicked()->name;
					if (name == L"Hub")
						con1Type = conType::Hub;
					else if (name == L"Group")
						con1Type = conType::Group;
					else if (name == L"N0")
						con1Type = conType::N0;
					else if (name == L"N1")
						con1Type = conType::N1;
					else if (name == L"N2")
						con1Type = conType::N2;
					else if (name == L"N3")
						con1Type = conType::N3;
				});

			gui.setDropList(mathOperatorList, [&](buttonList* bList)
				{
					const std::wstring& name = bList->lClicked()->name;
					for (int i = 0; i < (mathOperatorT::Decrement + 1); ++i)
						if (name == typeName::getMathOperatorShortSubtypeName(i))
						{
							creatingSubtype = i; 
							creatingMathOperatorSubtype = i;
						}
				});
		}
		
		visualNode* addNode()
		{
			nodePtr v1 = creator::createNode(creatingType, creatingSubtype);
			auto vNode1 = new visualNode(creator::createObject(1), v1);
			gui.workBoard.addNode(getObjectPtr<visualNode>(vNode1));
			connect(vNode1);
			return vNode1;
		}
		void connect(visualNode* vNode1)
		{
			groupIterator gi(sh.selectedGroup());
			do
			{
				auto vNode2 = getObject<visualNode>(gi.get());
				if ((!(vNode2)) || (vNode1->node() == vNode2->node()))
					continue;
				connect(vNode1, vNode2, con1Type, con2Type);
			} while (gi.stepForward());
		}
		static void connect(
			visualNode* vNode1, visualNode* vNode2, conType ct1, conType ct2)
		{
			assert((vNode1) && (vNode2));
			nodePtr v1 = vNode1->node().connection(0);
			nodePtr v2 = vNode2->node().connection(0);
			if (!(v1.exist() && v2.exist()))
				return;
			IterIterConnect(getIter(v1, ct1), getIter(v2, ct2));
		}
		static iterator getIter(nodePtr v1, conType c1)
		{
			if (c1 == conType::Hub)
				return firstEmptyHubPort(v1);
			if (c1 == conType::Group)
			{
				if (typeCompare(v1, nodeT::Group))
					return firstEmptyGroupPort(v1);
				else
					return firstEmptyHubPort(v1);
			}
			return portIterator(v1, 
				static_cast<int>(c1) - static_cast<int>(conType::N0));
		}
		
		void update()
		{

		}
		private:
		
		void setActiveTypeAndSubtype(char type, char subtype = 0)
		{
			creatingType = type;
			creatingSubtype = subtype;
		}
		void hideAllActiveDropLists()
		{
			mathOperatorList.hide();
		}
	};	
}
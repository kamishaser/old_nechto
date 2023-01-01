#pragma once
#include "GUI.h"
#include "buttonList.h"
#include "consistentGroup.h"
#include "dipSwitch.h"

namespace nechto::ide
{
	class settingList
	{
		
	public:
		dipSwitch testButton;
		consistentGroup settingListVisualGroup{ creator::createObject(0),
			L"список настроек", glm::vec2(100.f, 100.f) };

		buttonList settingButtonList;
		GUI& gui;
		settingList(GUI& g)
			:gui(g),
			testButton(g, L"testButton"),

			settingButtonList(creator::createObject(0), &settingListVisualGroup,
				L"список настроек", { testButton })
		{
			settingListVisualGroup.mode.horisontal = false;
			//settingListVisualGroup.mode.reverseDirection = true;
			gui.setDropList(settingButtonList, [&](buttonList* bList)
				{
					std::wcout << L"sClick" << std::endl;
					sharedButton* button = bList->lClicked();
					bList->resetLastClicked();
					if (button->name == L"testButton")
					{
						testButton.change();
					}
					
				}, false);
			settingButtonList.show(&gui.interfaceBoard);
		}
	};
}
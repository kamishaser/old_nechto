#pragma once
#include "display.h"
#include "visualGroup.h"
#include "consistentGroup.h"
#include "nodeBoard.h"
#include "textInputBox.h"
#include "textOut.h"
#include "button.h"
#include "namedExConGroup.h"
#include "buttonList.h"

namespace nechto::ide
{
	class GUI
	{
		namedExConGroup activeButtonList{ creator::createEntity(0), L"pressedButton" };
	public:
		display dp;

		nodeBoard workBoard;
		consistentGroup leftGroup{ creator::createEntity(0), L"leftGroup" };
		consistentGroup rightGroup{ creator::createEntity(0), L"rightGroup" };
		visualGroup center = { creator::createEntity(0), L"center"};

		nodeBoard interfaceBoard;
		consistentGroup topGroup{ creator::createEntity(0), L"topGroup"};
		consistentGroup bottomGroup{ creator::createEntity(0), L"bottomGroup" };

		visualNode cursoredParametrs{ creator::createEntity(0)};
		visualNode cursoredConnections{ creator::createEntity(0)};
		visualNode textBoxNode{ creator::createEntity(0)};
		textInputBox  textBox;
		
		std::map<std::wstring, namedExCon> parameterSet;//список параметров nechto
		
		namedExConGroup dropListGroup{ creator::createEntity(0), L"dropList" };

		GUI(const std::filesystem::path& path)
			:dp(path), textBox(textBoxNode.node())
		{
			//добавление групп на доски
			workBoard.addGroup(getEntityPtr<visualGroup>(&leftGroup));
			workBoard.addGroup(getEntityPtr<visualGroup>(&rightGroup));
			workBoard.addGroup(getEntityPtr<visualGroup>(&center));

			interfaceBoard.addGroup(getEntityPtr<visualGroup>(&topGroup));
			interfaceBoard.addGroup(getEntityPtr<visualGroup>(&bottomGroup));
			//установка режима позицианирования
			rightGroup.mode.rightAlignment = true;
			topGroup.mode.horisontal = true;
			bottomGroup.mode.horisontal = true;
			bottomGroup.mode.rightAlignment = true;
			//bottomGroup.mode.rightAlignment = true;
			//добавление элементов интерфейса
			interfaceBoard.addNode(getEntityPtr<visualNode>(&cursoredParametrs));
			interfaceBoard.addNode(getEntityPtr<visualNode>(&cursoredConnections));
			interfaceBoard.addNode(getEntityPtr<visualNode>(&textBoxNode));

			bottomGroup.addNode(getEntityPtr<visualNode>(&cursoredParametrs));
			bottomGroup.addNode(getEntityPtr<visualNode>(&cursoredConnections));
			bottomGroup.addNode(getEntityPtr<visualNode>(&textBoxNode));

			cursoredParametrs.nodeText = L"наведи на ноду для получения данных";

			setChainsPosition();
		}
		void setChainsPosition()
		{
			leftGroup.setPositionByStartPoint(glm::vec2(10, 10));
			rightGroup.setPositionByStartPoint(glm::vec2(dp.window.getSize().x - 10, 10));
			center.frame.setPositionByCenter(SFML_GLM(dp.window.getSize()) / 2.f);
			topGroup.setPositionByStartPoint(glm::vec2(10, 10));
			bottomGroup.setPositionByStartPoint(glm::vec2(10, dp.window.getSize().y - 10));

		}
		bool update()
		{
			setChainsPosition();

			groupPointer gi(interfaceBoard.vNodeGroup());
			do
			{
				auto vNode = getEntity<visualNode>(gi.get());
				if (vNode)
					vNode->frame.size = glm::vec2(0, 0);
			} while (gi.stepForward());

			return dp.update(workBoard, interfaceBoard, textBox);
		}
		void addButton(sharedButton* button, visualGroup* group)
		{
			auto vNode = getEntity<visualNode>(button->node().connection(0));
			if (!vNode)
			{
				vNode = new visualNode(creator::createEntity(1));
				NumNumConnect(button->node(), vNode->node(), 0, 0);
			}
			vNode->nodeText = button->name;
			interfaceBoard.addNode(vNode->node());
			group->addNode(vNode->node());
			
		}
		void deleteButton(sharedButton* button)
		{
			auto vNode = getEntity<visualNode>(button->node().connection(0));
			if (vNode)
				creator::deleteNode(vNode->node());
		}
		void hideAllActiveDropLists()
		{
			groupPointer gi(dropListGroup.getGroup());
			do
			{
				if (entityPtr<buttonList>::match(gi.get()))
				{
					entityPtr<buttonList>(gi.get())->hide();
				}
			} while (gi.stepForward());
		}
		void setbList(buttonList& list,
			buttonList::clickEventT clickEvent = nullptr)
		{
			auto vGroup = list.vNodeGroup();

			interfaceBoard.addGroup(getEntityPtr<visualGroup>(vGroup));
			//topGroup.addGroup(getEntityPtr<visualGroup>(vGroup));
			auto cGroup = dynamic_cast<consistentGroup*>(vGroup);
			if (cGroup)
				cGroup->mode.horisontal = true;
			list.clickEvent = clickEvent;
			list.show(&interfaceBoard);
		}
		void setDropList(buttonList& list,
			buttonList::clickEventT clickEvent = nullptr, bool closeAfterClick = true)
		{
			auto vGroup = list.vNodeGroup();
			interfaceBoard.addGroup(getEntityPtr(vGroup));
			auto cGroup = dynamic_cast<consistentGroup*>(vGroup);
			if (cGroup)
			{
				cGroup->distance = 5;
			}
			if (closeAfterClick)
			{
				list.clickEvent = [clickEvent](buttonList* list)
				{
					clickEvent(list);
					list->hide();
				};
			}
			else
				list.clickEvent = clickEvent;
			PointerHubConnect(firstEmptyGroupPort(dropListGroup.getGroup()), list.node());
		}
		bool isButtonActive(sharedButton* button)
		{
			if (button == nullptr)
				return false;
			return activeButtonList.contains(button->content());
		}
		void activeButton(sharedButton* button)
		{
			activeButtonList.addNodeToNearestPort(button->node().connection(0));
		}
		void resetButton(sharedButton* button)
		{
			nearestDisconnect(activeButtonList.node().connection(0),
				button->content());
		}
	};
}
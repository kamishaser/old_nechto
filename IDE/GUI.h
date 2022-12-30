#pragma once
#include "display.h"
#include "visualGroup.h"
#include "consistentGroup.h"
#include "nodeBoard.h"
#include "textInputBox.h"
#include "textOut.h"
#include "button.h"
#include "namedExConGroup.h"

namespace nechto::ide
{
	class GUI
	{
	public:
		display dp;

		nodeBoard workBoard;
		consistentGroup leftGroup{ creator::createObject(0), L"leftGroup" };
		consistentGroup rightGroup{ creator::createObject(0), L"rightGroup" };
		visualGroup center = { creator::createObject(0), L"center"};

		nodeBoard interfaceBoard;
		consistentGroup topGroup{ creator::createObject(0), L"topGroup"};
		consistentGroup bottomGroup{ creator::createObject(0), L"bottomGroup" };

		visualNode cursoredParametrs{ creator::createObject(0)};
		visualNode cursoredConnections{ creator::createObject(0)};
		visualNode textBoxNode{ creator::createObject(0)};
		textInputBox  textBox;


		namedExConGroup activeButton{ creator::createObject(0), L"pressedButton" };

		GUI()
			:dp(), textBox(textBoxNode.node())
		{
			//добавление групп на доски
			workBoard.addGroup(getObjectPtr<visualGroup>(&leftGroup));
			workBoard.addGroup(getObjectPtr<visualGroup>(&rightGroup));
			workBoard.addGroup(getObjectPtr<visualGroup>(&center));

			interfaceBoard.addGroup(getObjectPtr<visualGroup>(&topGroup));
			interfaceBoard.addGroup(getObjectPtr<visualGroup>(&bottomGroup));
			//установка режима позицианирования
			rightGroup.mode.rightAlignment = true;
			topGroup.mode.horisontal = true;
			bottomGroup.mode.horisontal = true;
			bottomGroup.mode.rightAlignment = true;
			//bottomGroup.mode.rightAlignment = true;
			//добавление элементов интерфейса
			interfaceBoard.addNode(getObjectPtr<visualNode>(&cursoredParametrs));
			interfaceBoard.addNode(getObjectPtr<visualNode>(&cursoredConnections));
			interfaceBoard.addNode(getObjectPtr<visualNode>(&textBoxNode));

			bottomGroup.addNode(getObjectPtr<visualNode>(&cursoredParametrs));
			bottomGroup.addNode(getObjectPtr<visualNode>(&cursoredConnections));
			bottomGroup.addNode(getObjectPtr<visualNode>(&textBoxNode));

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

			groupIterator gi(interfaceBoard.vNodeGroup());
			do
			{
				auto vNode = getObject<visualNode>(gi.get());
				if (vNode)
					vNode->frame.size = glm::vec2(0, 0);
			} while (gi.stepForward());

			return dp.update(workBoard, interfaceBoard, textBox);
		}
		void addButton(sharedButton* button, visualGroup* group)
		{
			auto vNode = getObject<visualNode>(button->node().connection(0));
			if (!vNode)
			{
				vNode = new visualNode(creator::createObject(1));
				NumNumConnect(button->node(), vNode->node(), 0, 0);
			}
			vNode->nodeText = button->name;
			interfaceBoard.addNode(vNode->node());
			group->addNode(vNode->node());
			
		}
		void resetButton(sharedButton* button)
		{
			auto vNode = getObject<visualNode>(button->node().connection(0));
			if (vNode)
				creator::deleteNode(vNode->node());
		}
	};
}
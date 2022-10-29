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
		consistentGroup leftGroup{ newExObjNode(0), L"leftGroup" };
		consistentGroup rightGroup{ newExObjNode(0), L"rightGroup" };
		visualGroup center = { newExObjNode(0), L"center"};

		nodeBoard interfaceBoard;
		consistentGroup topGroup{ newExObjNode(0), L"topGroup"};
		consistentGroup bottomGroup{ newExObjNode(0), L"bottomGroup" };

		visualNode cursoredParametrs{ newExObjNode(0) };
		visualNode cursoredConnections{ newExObjNode(0) };
		visualNode textBoxNode{ newExObjNode(0) };
		visualNode consoleOut{ newExObjNode(0) };
		textInputBox  textBox;


		namedExConGroup activeButton{ newExObjNode(0), L"pressedButton" };

		GUI()
			:dp(), textBox(textBoxNode.get())
		{
			//добавление групп на доски
			workBoard.addGroup(&leftGroup);
			workBoard.addGroup(&rightGroup);
			workBoard.addGroup(&center);

			interfaceBoard.addGroup(&topGroup);
			interfaceBoard.addGroup(&bottomGroup);
			//установка режима позицианирования
			rightGroup.mode.rightAlignment = true;
			topGroup.mode.horisontal = true;
			bottomGroup.mode.horisontal = true;
			bottomGroup.mode.rightAlignment = true;
			//bottomGroup.mode.rightAlignment = true;
			//добавление элементов интерфейса
			interfaceBoard.addNode(&cursoredParametrs);
			interfaceBoard.addNode(&cursoredConnections);
			interfaceBoard.addNode(&textBoxNode);
			interfaceBoard.addNode(&consoleOut);

			bottomGroup.addNode(&cursoredParametrs);
			bottomGroup.addNode(&cursoredConnections);
			bottomGroup.addNode(&textBoxNode);
			bottomGroup.addNode(&consoleOut);
			consoleText = &consoleOut.nodeText;

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
				auto vNode = visualNode::getByNode(gi.get());
				if (vNode)
					vNode->frame.size = glm::vec2(0, 0);
			} while (gi.stepForward());

			return dp.update(workBoard, interfaceBoard, textBox);
		}
		void addButton(sharedButton* button, visualGroup* group)
		{
			auto vNode = visualNode::getByNode(button->getConnection(0));
			if (!vNode)
			{
				vNode = new visualNode(newExObjNode());
				NumNumConnect(button->get(), vNode->get(), 0, 0);
			}
			vNode->nodeText = button->name;
			interfaceBoard.addNode(vNode);
			group->addNode(vNode);
			
		}
		void resetButton(sharedButton* button)
		{
			auto vNode = visualNode::getByNode(button->getConnection(0));
			if (vNode)
				deleteNode(vNode->get());
		}
	};
}
#pragma once
#include "ideDisplay.h"
#include "nodeBoard.h"
#include "namedExCon.h"
#include "textOut.h"

namespace nechto::ide
{
	class userH
	{
	public:
		ideDisplay* display;
		nodeBoard* nBoard;

		namedExCon cursored;
		glm::vec2 lastCPos;


		//externalObject moveCursored =
		//	externalObject(createVariable(0ll), L"moveCursored");
		//glm::vec2 offset;
		bool last = false;
	
		userH(nodeBoard* nbn, ideDisplay* idn)
		:nBoard(nbn), display(idn),
			cursored(L"#nechtoIde.cursored"),
			lastCPos(display->getCursorPosition()){}

		void update()
		{
			updateCursored();
			updateHighlighted();
			updateMoved();
		}
	private:
		void updateCursored()
		{
			nodePtr last = cursored.getConnection(0);
			if (last.exist())
				numDisconnect(cursored.get(), 0);
			groupIterator i1(nBoard->vNodeGroup());
			do
			{
				auto* vNode = visualNode::getByNode(i1.get());
				if (!vNode)
					continue;
				if (vNode->frame.contains(display->getCursorPosition()))
				{
					if (vNode->get().exist())
					{
						if (vNode->get() != last)
						{
							std::wcout << L"cursored: " << nodeProperties(vNode->getConnection(0)) << std::endl;
						}
						NumHubConnect(cursored.get(), vNode->get(), 0);
					}
				}
			} while (i1.stepForward());
		}
		bool leftButtonWasPressed = false;
		milliseconds clickStartTime = 0ms;
		const milliseconds moveTrigger = 200ms;
		void updateHighlighted()
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (!leftButtonWasPressed &&
					cursored.getConnection(0).exist() &&
					cursored.getConnection(0)->connection[0].load().exist())
				{
					clickStartTime = currentTime();
					nodePtr cursoredNode = 
						cursored.getConnection(0)->connection[0];
					std::wcout << connectionsList(cursoredNode) << std::endl;
				}
				leftButtonWasPressed = true;
				if (!movingNode && cursored.getConnection(0).exist() &&
					(currentTime() - clickStartTime > moveTrigger))
				{
					movingNode = visualNode::getByNode(
						cursored.getConnection(0));
					mouseMovingNodeOffset = 
						display->getCursorPosition() - 
						movingNode->frame.position;
				}
			}
			else
			{
				movingNode = nullptr;
				leftButtonWasPressed = false;
			}
		}
		visualNode* movingNode= nullptr;
		glm::vec2 mouseMovingNodeOffset;
		
		void updateMoved()
		{
			if (movingNode)
			{
				movingNode->target =
					display->getCursorPosition() - mouseMovingNodeOffset;
			}
		}
	};
}
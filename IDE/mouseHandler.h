#pragma once
#include "visualNode.h"
#include "GUI.h"
#include "namedExCon.h"
#include "periodLimiter.h"
#include "textOut.h"
#include "button.h"
#include "buttonList.h"
#include "selectHandler.h"
#include "keyboardHandler.h"

namespace nechto::ide
{
	class mouseHandler
	{
		GUI& gui;
		selectHandler& sh;
		keyboardHandler& keyboard;

		namedExCon cursor{ L"mouseCursor" };
		namedExCon lClicked{ L"lastClickedNode" };
	public:
		
		sharedButton leftButton{ L"leftMouseButton" };
		sharedButton middleButton{ L"middleMouseButton" };
		sharedButton rightButton{ L"rightMouseButton" };

		mouseHandler(GUI& g, 
			keyboardHandler& keyboardH, selectHandler& selectH)
			:gui(g), keyboard(keyboardH), sh(selectH){}

		glm::vec2 cursorPosition() const
		{
			return SFML_GLM(sf::Mouse::getPosition(gui.dp.window));
		}
		visualNode* cursored() const
		{
			return visualNode::getByNode(cursor.getConnection(0));
		}
		void setCursored(visualNode* cursoredNode)
		{
			numDisconnect(cursor.get(), 0);
			if(cursoredNode)
				NumHubConnect(cursor.get(), cursoredNode->get(), 0);
		}
		visualNode* lastClicked() const
		{
			return visualNode::getByNode(lClicked.getConnection(0));
		}
		void click(visualNode* clickedNode)
		{
			numDisconnect(lClicked.get(), 0);
			if(clickedNode)
				NumHubConnect(lClicked.get(), clickedNode->get(), 0);
		}
		void update()
		{
			if (gui.dp.window.hasFocus() && 
				gui.dp.windowRect().contains(cursorPosition()))
			{
				leftButton.update(sf::Mouse::isButtonPressed(sf::Mouse::Left), 
					cursored());
				middleButton.update(sf::Mouse::isButtonPressed(sf::Mouse::Middle), 
					cursored());
				rightButton.update(sf::Mouse::isButtonPressed(sf::Mouse::Right), 
					cursored());

				setCursored(nullptr);
				updateCursored(&gui.interfaceBoard, cursorPosition());
				if (cursored() == nullptr)
					updateCursored(&gui.workBoard, cursorPosition());

				updateLeftButton();
			}
		}
		
	private:
		void updateCursored(const nodeBoard* nBoard, glm::vec2 cursorPosition)
		{
			groupIterator i1(nBoard->vNodeGroup());
			do
			{
				auto* vNode = visualNode::getByNode(i1.get());
				if (!vNode)
					continue;
				if (vNode->frame.contains(cursorPosition))
					setCursored(vNode);
			} while (i1.stepForward());
		}

		glm::vec2 lastCursorPosition = cursorPosition();//позиция курсора при предыдущем обновлении
		const milliseconds pressTrigger = 175ms;

		void select(visualNode* vNode)
		{
			if (!sh.contains(vNode))
				sh.select(vNode);
			else
				sh.deselect(vNode);
		}

		//обработка действий левой кнопкой мыши (в основном)
		void updateLeftButton()
		{
			auto vNode = visualNode::getByNode(leftButton.content());
			bool clicked = leftButton.bClickEvent();
			if (vNode == nullptr)//если мышь ни на что не наведена
			{
				if (clicked && !(keyboard.control() || keyboard.shift()))
					sh.deselectAll();
				return;
			}
			
			
			if (vNode->getNodeBoard() == gui.interfaceBoard.get())
			{
				if (lastClicked() && (vNode->get() != lastClicked()->get()))
				{
					updateButtonByNode(lastClicked(), false);
				}
				updateButtonByNode(vNode, leftButton.isPressed());
			}
			else if (vNode->getNodeBoard() == gui.workBoard.get())
			{
				updateWorkNode(vNode, clicked);
				if (clicked)
					click(vNode);
			}
		}
		void updateWorkNode(visualNode* vNode, bool click)
		{
			if (click)
			{
				if (!sh.select(vNode))
					sh.deselect(vNode);
				gui.textBox.reset();
			}
			else if (leftButton.pressTime() > pressTrigger)
			{
				if (leftButton.isPressed())
				{
					glm::vec2 offset = cursorPosition() - lastCursorPosition;
					if (!sh.contains(vNode))
						moveAllSelected(offset);
					vNode->frame.position += offset;
				}
				else if (leftButton.eClickEvent())
					if (!sh.select(vNode))
						sh.deselect(vNode);
					else;
				else;
			}
			else if (leftButton.eClickEvent())
			{
				bool selected = sh.contains(vNode);
				if (!(keyboard.shift() || keyboard.control()))
					sh.deselectAll();
				if (selected)
					sh.select(vNode);
			}
			lastCursorPosition = cursorPosition();
		}
		void moveAllSelected(glm::vec2 offset)
		{
			groupIterator gi(sh.selectedGroup());
			do
			{
				auto vNode = visualNode::getByNode(gi.get());
				if (vNode)
				{
					vNode->frame.position += offset;
				}
			} while (gi.stepForward());
		}
		void updateButtonByNode(visualNode* vNode, bool status)
		{
			auto button =
				sharedButton::getByNode(vNode->getConnection(0));
			if (button)
			{
				button->update(status);
				auto bList = buttonList::getByNode(button->getList());
				if (bList && bList->choiseMode)
					if (button->bClickEvent())
					{
						bList->click(button);
						print(std::wstring(L"clickButton ") + to_string(button->get()));
					}
			}
		}
	};
}
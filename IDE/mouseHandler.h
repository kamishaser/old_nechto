#pragma once
#include "visualNode.h"
#include "display.h"
#include "namedExCon.h"
#include "periodLimiter.h"
#include "textOut.h"
#include "button.h"
#include "selectHandler.h"
#include "keyboardHandler.h"

namespace nechto::ide
{
	class mouseHandler
	{
		display& dp;
		selectHandler& sh;
		keyboardHandler& keyboard;
	public:
		namedExCon cursor;

		sharedButton leftButton;
		sharedButton middleButton;
		sharedButton rightButton;

		mouseHandler(display& dplay, 
			keyboardHandler& keyboardH, selectHandler& selectH)
			:dp(dplay), keyboard(keyboardH), sh(selectH),
			cursor(L"mouseCursor"),
			leftButton(L"leftMouseButton"),
			middleButton(L"middleMouseButton"),
			rightButton(L"rightMouseButton")
		{
			NumNumConnect(cursor.get(), newNode(node::Variable, 1), 3, 0);//x
			NumNumConnect(cursor.get(), newNode(node::Variable, 1), 4, 0);//y
		}
		~mouseHandler()
		{
			deleteNode(cursor.getConnection(3));
			deleteNode(cursor.getConnection(4));
		}

		glm::vec2 cursorPosition() const
		{
			return SFML_GLM(sf::Mouse::getPosition(dp.window));
		}
		nodePtr x()
		{
			return cursor.getConnection(3);
		}
		nodePtr y()
		{
			return cursor.getConnection(4);
		}
		nodePtr cursored()
		{
			return cursor.getConnection(0);
		}
		/*возвращает nullptr, если нажата нода на workBoard или ничего, 
		иначе на interfaceBoard*/
		visualNode* update()
		{
			if (dp.window.hasFocus() && dp.windowRect().contains(cursorPosition()))
			{
				x()->setData<i64>(cursorPosition().x);
				y()->setData<i64>(cursorPosition().y);

				leftButton.update(
					sf::Mouse::isButtonPressed(sf::Mouse::Left), cursored());
				middleButton.update(
					sf::Mouse::isButtonPressed(sf::Mouse::Middle), cursored());
				rightButton.update(
					sf::Mouse::isButtonPressed(sf::Mouse::Right), cursored());

				updateCursored(&dp.interfaceBoard, cursorPosition());
				if (!cursored().exist())
					updateCursored(&dp.workBoard, cursorPosition());
				return updateLeftButton();
			}
		}
		
	private:
		void updateCursored(const nodeBoard* nBoard, glm::vec2 cursorPosition)
		{
			nodePtr last = cursor.getConnection(0);
			if (last.exist())
				numDisconnect(cursor.get(), 0);
			groupIterator i1(nBoard->vNodeGroup());
			do
			{
				auto* vNode = visualNode::getByNode(i1.get());
				if (!vNode)
					continue;
				if (vNode->frame.contains(cursorPosition))
				{
					if (vNode->get().exist())
					{
						NumHubConnect(cursor.get(), vNode->get(), 0);
					}
				}
			} while (i1.stepForward());
		}

		glm::vec2 lastCursorPosition = cursorPosition();//позиция курсора при предыдущем обновлении
		const milliseconds pressTrigger = 175ms;

		void select(visualNode* vNode)
		{
			if (!sh.contains(vNode))
			{
				
				sh.select(vNode);
			}
			else
				sh.deselect(vNode);
		}
		//обработка действий левой кнопкой мыши (в основном)
		visualNode* updateLeftButton()
		{
			auto vNode = visualNode::getByNode(leftButton.content());
			if (vNode == nullptr)
			{
				if (leftButton.bClickEvent() &&
					!(keyboard.control() || keyboard.shift()))
					sh.deselectAll();
				return nullptr;
			}
			if(vNode->getNodeBoard() != dp.workBoard.get())
			{
				leftButton.bClickEvent();
				return vNode;
			}

			if (leftButton.bClickEvent())
			{
				if (!sh.select(vNode))
					sh.deselect(vNode);
				dp.textBox.reset();
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
			else if(leftButton.eClickEvent())
			{
				bool selected = sh.contains(vNode);
				if (!(keyboard.shift() || keyboard.control()))
					sh.deselectAll();
				if (selected)
					sh.select(vNode);
			}
			lastCursorPosition = cursorPosition();
			return nullptr;
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
	};
}
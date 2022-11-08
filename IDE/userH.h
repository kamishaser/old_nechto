#pragma once
#include "nodeBoard.h"
#include "namedExCon.h"
#include "display.h"
#include "mouseHandler.h"
#include "keyboardHandler.h"
#include "selectHandler.h"
#include "editor.h"

namespace nechto::ide
{
	//обработчик пользовательских действий
	class userH
	{
	public:
		GUI& gui;
		editor& ed;
		keyboardHandler keyboard;
		selectHandler selectH;
		mouseHandler mouse;

		sharedButton testButton;
		
		userH(GUI& g, editor& e)
		:gui(g), ed(e), selectH(), mouse(g, keyboard, selectH),
		testButton(L"testButton")
		{
			gui.addButton(&testButton, &gui.bottomGroup);
		}

		void updateTestButton()
		{
			if (testButton.bClickEvent())
			{
				if(gui.activeButton.contains(testButton.content()))
					disconnect(gui.activeButton.getConnection(0), 
						testButton.content());
				else
					gui.activeButton.addNode(testButton.getConnection(0));
			}
		}

		void update()
		{
			mouse.update();
			if (mouse.rightButton.bClickEvent())
			{
				auto vNode1 =
					visualNode::getByNode(mouse.rightButton.content());
				if (vNode1)
					editText(vNode1);
				else
					gui.textBox.reset();
			}
			if (mouse.middleButton.bClickEvent())
				addNode();
			if (!gui.textBox.hasFocus())
			{
				keyboard.update();
				if (keyboard[sf::Keyboard::N].bClickEvent())//создание
					addNode();
				if (keyboard[sf::Keyboard::Delete].bClickEvent())//удаление
					deleteAllSelected();
				if (keyboard[sf::Keyboard::C].bClickEvent())//подключение
					ed.connect(visualNode::getByNode(selectH.lastSelected()));
				if (keyboard[sf::Keyboard::D].bClickEvent())//отключение
					disconnectSelected();
				if (keyboard[sf::Keyboard::A].bClickEvent())//выделение всех
					selectH.selectGroup(gui.workBoard.vNodeGroup());
				if (keyboard[sf::Keyboard::R].bClickEvent())//рандомное смещение всех
					randomOfsetSelected();
				if (keyboard[sf::Keyboard::S].bClickEvent())//рандомное смещение всех
					operateLSelected();
				if (keyboard[sf::Keyboard::Tab].isPressed())//описание соединений
					if (mouse.cursored())
						gui.cursoredParametrs.nodeText =
						connectionsList(mouse.cursored()->getConnection(0));
					else;
				else
					gui.cursoredParametrs.nodeText = std::to_wstring(nodeStorage::terminal.numberOfNodes);
			}
			else
			{
				for (auto& i : keyboard.keyBoardKeys)
					i.bClickEvent();
				mouse.rightButton.bClickEvent();
				mouse.middleButton.bClickEvent();
			}
			updateTestButton();
		}
		void addNode()
		{
			auto vNode = ed.addNode();
			vNode->frame.setPositionByCenter(mouse.cursorPosition());
			editText(vNode);
		}
		void editText(visualNode* vNode1)
		{
			nodePtr v1 = vNode1->getConnection(0);
			if (v1.exist())
			{
				gui.textBox.focus(vNode1);
			}
		}
		void deleteAllSelected()
		{
			groupIterator gi(selectH.selectedGroup());
			do
			{
				if (gi.get().exist())
				{
					if (gi.get()->connection[0].load().exist())
						deleteNode(gi.get()->connection[0]);
					deleteNode(gi.get());
				}
			} while (gi.stepForward());
		}

		void connectSelected(visualNode* vNode1)
		{
			if (vNode1 == nullptr)
				return;
			groupIterator gi(selectH.selectedGroup());
			do
			{
				auto vNode2 = visualNode::getByNode(gi.get());
				if (vNode2)
				{
					nodePtr v1 = vNode1->getConnection(0);
					nodePtr v2 = vNode2->getConnection(0);
					if (v1 != v2)
					{
						if (v1.exist() && v2.exist() && !hasConnection(v1, v2))
						{
							HubHubConnect(v1, v2);
						}
						if (!gui.workBoard.connected(vNode1, vNode2))
						{
							auto vConnection = new visualConnection(
								newExObjNode(), vNode1, vNode2);
							gui.workBoard.addConnection(vConnection);
						}
					}
				}
			} while (gi.stepForward());
		}
		void disconnectSelected()
		{
			if (!selectH.lastSelected().exist())
				return;
			groupIterator gi(selectH.selectedGroup());
			do
			{
				if (gi.get().exist() && gi.get() != selectH.lastSelected())
				{
					nodePtr v1 = gi.get()->connection[0];
					nodePtr v2 = selectH.lastSelected()->connection[0];
					if (v1.exist() && v2.exist())
						disconnect(v1, v2);
				}
			} while (gi.stepForward());
		}
		void randomOfsetSelected()
		{
			groupIterator gi(selectH.selectedGroup());
			do
			{
				auto vNode = visualNode::getByNode(gi.get());
				if (vNode)
					vNode->frame.position += randomOffset(100);
			} while (gi.stepForward());
		}
		void operateLSelected()
		{
			nodePtr lSelected = selectH.lastSelected();
			if (!lSelected.exist())
				return;
			lSelected = lSelected->connection[0];
			if (!lSelected.exist())
				return;
			if (!(isAction(lSelected) && check(lSelected)))
				return;
			
			lSelected = step(lSelected);
			if (!lSelected.exist())
				return;
			connectionIterator ci(lSelected);
			do
			{
				auto vNode = visualNode::getByNode(ci.get());
				if (vNode)
				{
					selectH.select(vNode);
					break;
				}
			} while (ci.stepForward());
		}
	};
}
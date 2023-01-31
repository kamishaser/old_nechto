#pragma once
#include "nodeBoard.h"
#include "namedExCon.h"
#include "display.h"
#include "mouseHandler.h"
#include "keyboardHandler.h"
#include "selectHandler.h"
#include "editor.h"
#include "settingList.h"


namespace nechto::ide
{
	//обработчик пользовательских действий
	class userH
	{
	public:
		GUI& gui;
		editor& ed;
		settingList& settings;
		keyboardHandler keyboard;
		selectHandler selectH;
		mouseHandler mouse;

		sharedButton testButton;
		
		userH(GUI& g, editor& e, settingList& sl)
		:gui(g), ed(e), selectH(), mouse(g, keyboard, selectH), settings(sl),
		testButton(L"testButton")
		{
			gui.addButton(&testButton, &gui.bottomGroup);
		}

		void updateTestButton()
		{
			if (testButton.bClickEvent())
			{
				if (gui.isButtonActive(&testButton))
					gui.resetButton(&testButton);
				else
					gui.activeButton(&testButton);
			}
		}

		void update()
		{
			mouse.update();
			if (mouse.rightButton.bClickEvent())
			{
				auto vNode1 =
					getEntity<visualNode>(mouse.rightButton.content());
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
					ed.connect(getEntity<visualNode>(selectH.lastSelected()));
				if (keyboard[sf::Keyboard::D].bClickEvent())//отключение
					disconnectSelected();
				if (keyboard[sf::Keyboard::A].bClickEvent())//выделение всех
					selectH.selectGroup(gui.workBoard.vNodeGroup());
				if (keyboard[sf::Keyboard::R].bClickEvent())//рандомное смещение всех
					randomOfsetSelected();
				if (keyboard[sf::Keyboard::T].bClickEvent())//ввод текста для команды
					gui.textBox.focus(L"введите текст", nullptr);
				//if (keyboard[sf::Keyboard::S].bClickEvent())//выполнение помледнего выделенного действия
				//	operateLSelected();
				if (keyboard[sf::Keyboard::Tab].isPressed())//описание соединений
					if (mouse.cursored())
						gui.cursoredParametrs.nodeText =
						connectionsList(mouse.cursored()->node().connection(0));
					else;
				else if (settings.testButton.test())
					gui.cursoredParametrs.nodeText = getStringNumberOfNodesOfAllTypes();
				else
					gui.cursoredParametrs.nodeText =
					std::to_wstring(nodeStorage::terminal.numberOfNodes) + L"\n" +
					std::to_wstring(entity::numberOfEntitys);
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
			nodePtr v1 = vNode1->node().connection(0);
			if (v1.exist())
			{
				gui.textBox.focus(L"введите данные ноды", vNode1);
			}
		}
		void deleteAllSelected()
		{
			groupPointer gi(selectH.selectedGroup());
			do
			{
				if (gi.get().exist())
				{
					if (gi.get().connection(0).exist())
						creator::deleteNode(gi.get().connection(0));
					creator::deleteNode(gi.get());
				}
			} while (gi.stepForward());
		}

		void connectSelected(visualNode* vNode1)
		{
			if (vNode1 == nullptr)
				return;
			groupPointer gi(selectH.selectedGroup());
			do
			{
				auto vNode2 = getEntity<visualNode>(gi.get());
				if (vNode2)
				{
					nodePtr v1 = vNode1->node().connection(0);
					nodePtr v2 = vNode2->node().connection(0);
					if (v1 != v2)
					{
						if (v1.exist() && v2.exist() && !hasConnection(v1, v2))
						{
							HubHubConnect(v1, v2);
						}
						if (!gui.workBoard.connected(getEntityPtr(vNode1), 
							getEntityPtr(vNode2)))
						{
							gui.workBoard.addConnection(visualConnection::create(
								getEntityPtr(vNode1),
								getEntityPtr(vNode2)));
						}
					}
				}
			} while (gi.stepForward());
		}
		void disconnectSelected()
		{
			if (!selectH.lastSelected().exist())
				return;
			groupPointer gi(selectH.selectedGroup());
			do
			{
				if (gi.get().exist() && gi.get() != selectH.lastSelected())
				{
					nodePtr v1 = gi.get().connection(0);
					nodePtr v2 = selectH.lastSelected().connection(0);
					if (v1.exist() && v2.exist())
						nearestDisconnect(v1, v2);
				}
			} while (gi.stepForward());
		}
		void randomOfsetSelected()
		{
			groupPointer gi(selectH.selectedGroup());
			do
			{
				auto vNode = getEntity<visualNode>(gi.get());
				if (vNode)
					vNode->frame.position += randomOffset(100);
			} while (gi.stepForward());
		}
		/*void operateLSelected()
		{
			nodePtr lSelected = selectH.lastSelected();
			if (!lSelected.exist())
				return;
			lSelected = lSelected.connection(0);
			if (!lSelected.exist())
				return;
			if (!(isAction(lSelected) && check(lSelected)))
				return;
			
			lSelected = step(lSelected);
			if (!lSelected.exist())
				return;
			portPointer ci(lSelected);
			do
			{
				auto vNode = getEntity<visualNode>(ci.get());
				if (vNode)
				{
					selectH.select(vNode);
					break;
				}
			} while (ci.stepForward());
		}*/
	};
}
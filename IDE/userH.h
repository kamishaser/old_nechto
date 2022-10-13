#pragma once
#include "nodeBoard.h"
#include "namedExCon.h"
#include "display.h"
#include "mouseHandler.h"
#include "keyboardHandler.h"
#include "selectHandler.h"

namespace nechto::ide
{
	//обработчик пользовательских действий
	class userH
	{
	public:
		display& dp;
		keyboardHandler keyboard;
		selectHandler selectH;
		mouseHandler mouse;
		
		
		
		userH(display& dplay)
		:dp(dplay), selectH(dplay), mouse(dplay, keyboard, selectH){}

		

		enum ActiveAction
		{
			None,
			Save,//ввод сохранения
			Load,

		};
		ActiveAction aAction = None;
		std::wstring aString;
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
					dp.textBox.reset();
			}
			if (mouse.middleButton.bClickEvent())
				editText(addTextNodeAndHubHubConnectToSelected());
			if (!dp.textBox.hasFocus())
			{
				keyboard.update();
				if (keyboard[sf::Keyboard::N].bClickEvent())//создание
					editText(addTextNodeAndHubHubConnectToSelected());
				if (keyboard[sf::Keyboard::Delete].bClickEvent())//удаление
					deleteAllSelected();
				if (keyboard[sf::Keyboard::C].bClickEvent())//подключение
					HHconnectSelected();
				if (keyboard[sf::Keyboard::D].bClickEvent())//отключение
					disconnectSelected();
				if (keyboard[sf::Keyboard::S].bClickEvent())//сохранение
				{
					aAction = Save;
					dp.textBox.focus(&aString);
				}
				if (keyboard[sf::Keyboard::L].bClickEvent())//загрузка
				{
					aAction = Load;
					dp.textBox.focus(&aString);
				}
				if (keyboard[sf::Keyboard::A].bClickEvent())//выделение всех
					selectH.selectGroup(dp.workBoard.vNodeGroup());
				if (keyboard[sf::Keyboard::R].bClickEvent())//рандомное смещение всех
					randomOfsetSelected();
				if (keyboard[sf::Keyboard::Tab].isPressed())//рандомное смещение всех
					dp.cursoredParametrs.nodeText =
					connectionsList(mouse.cursored());
				else
					dp.cursoredParametrs.nodeText.clear();
				if (!aString.empty())
				{
					switch (aAction)
					{
					case nechto::ide::userH::None:
						break;
					case nechto::ide::userH::Save:
						dp.save(aString);
						break;
					case nechto::ide::userH::Load:
						dp.load(aString);
						break;
					default:
						break;
					}
					aAction = None;
					aString.clear();
				}
				
			}
			else
			{
				for (auto& i : keyboard.keyBoardKeys)
					i.bClickEvent();
				mouse.rightButton.bClickEvent();
				mouse.middleButton.bClickEvent();
			}
		}
		
		void editText(visualNode* vNode1)
		{
			dp.textBox.reset();
			nodePtr v1 = vNode1->getConnection(0);
			if (v1.exist() && typeCompare(v1, node::Text))
			{
				NumHubConnect(dp.textBox.get(), vNode1->get(), 1);
				dp.textBox.iText = text::get(v1);
				dp.textBox.focus();
			}
		}
		visualNode* addTextNodeAndHubHubConnectToSelected()
		{
			nodePtr v1 = newNode(node::Text);
			auto vNode1 = new visualNode(newExObjNode(), v1);
			vNode1->frame.setPositionByCenter(mouse.cursorPosition());
			dp.workBoard.addNode(vNode1);
			groupIterator gi(selectH.selectedGroup());
			do
			{
				auto vNode2 = visualNode::getByNode(gi.get());
				if (vNode2)
				{
					dp.workBoard.addConnection(new visualConnection(
						newExObjNode(), vNode1, vNode2));
					nodePtr v2 = vNode2->getConnection(0);
					if (v2.exist())
						HubHubConnect(v1, v2);
				}

			} while (gi.stepForward());
			return vNode1;
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
		
		void HHconnectSelected()
		{
			auto vNode1 = visualNode::getByNode(selectH.lastSelected());
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
						if (!dp.workBoard.connected(vNode1, vNode2))
						{
							auto vConnection = new visualConnection(
								newExObjNode(), vNode1, vNode2);
							dp.workBoard.addConnection(vConnection);
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
	};
}
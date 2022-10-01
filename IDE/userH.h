#pragma once
#include "nodeBoard.h"
#include "namedExCon.h"
#include "textOut.h"
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

		

		/*enum Stage
		{
			base,

		};*/
		void update()
		{
			mouse.update();
			keyboard.update();
			if (!dp.textBox.hasFocus())
			{
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
				if (keyboard[sf::Keyboard::Delete].bClickEvent())
					deleteAllSelected();
				if (keyboard[sf::Keyboard::C].bClickEvent())
					HHconnectSelected();
				if (keyboard[sf::Keyboard::D].bClickEvent())
					disconnectSelected();
				if (keyboard[sf::Keyboard::Escape].bClickEvent())
					dp.textBox.reset();
			}
		}
		void editText(visualNode* vNode1)
		{
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
	};
}
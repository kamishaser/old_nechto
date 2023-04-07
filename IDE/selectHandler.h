#pragma once
#include "ideStructurePack.h"
#include "event.h"
#include "visual.h"

namespace nechto::ide
{
	class selectHandler : public singleConnectedEntity
	{
	public:
		selectHandler()
		{
			eConnect(creator::createEntity(entityT::singleConnection));
			fabricate(sPack::selectHandler::getPlan(), node());
		}
		void finalConnect(nodePtr ideNode)
		{
			NumNumConnect(-sPack::nechtoIde::selectHanlder / ideNode,
				node(), sPack::nechtoIde::selectHanlder.last(), 3);//подключение к системе
			nodePtr mouse = sPack::nechtoIde::mouse / ideNode;
			nodePtr mClickSubscription =
				sPack::selectHandler::mClickSubscription / node();
			subscribeToSource(
				mClickSubscription, sPack::mouse::eventSource / mouse);
			subscribeToType(
				mClickSubscription, sPack::mouse::eventType_ButtonClicked / mouse);
			NumHubConnect(-sPack::selectHandler::mouse / node(),
				mouse, sPack::selectHandler::mouse.last());
			nodePtr illh = sPack::nechtoIde::illuminationHandler / ideNode;
			assert(illh.exist());
			nodePtr eventType = sPack::illuminationHandler::eventType_illGExchenge / illh;
			nodePtr selectGroup = sPack::illuminationHandler::selectGroup / illh;
			NumHubConnect(-sPack::selectHandler::selectGroup / node(),
				selectGroup, sPack::selectHandler::selectGroup.last());
			NumHubConnect(-sPack::selectHandler::eventType_illGExchenge / node(),
				eventType, sPack::selectHandler::eventType_illGExchenge.last());
		}
		void update()
		{
			nodePtr mouse = sPack::selectHandler::mouse / node();
			nodePtr buttonNode = sPack::mouse::leftButton / mouse;
			nodePtr curVel = sPack::mouse::curVel / mouse;
			i64 buttonStatus = 0;
			buttonNode >> buttonStatus;
			buttonStatus = ((buttonStatus % 2) == 0);
			nodePtr subscription = sPack::selectHandler::mClickSubscription / node();
			for (nodePtr event; +(event = readEvent(subscription));)
			{
				if (+event && sPack::event::content / event == buttonNode && buttonStatus)
				{
					if (+curVel)
					{
						nodePtr nodeBoard = sPack::vNode::nodeBoard / curVel;
						bool atWordBoard = reversePort(portPointer(nodeBoard, 3)).getLocalPos() == 0;
						if (atWordBoard)
							changeSelection(curVel);
					}
					else if (!shift())
						delesectAll();
				}
			}
		}
	private:
		bool shift()const
		{
			return
				sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
		}
		void select(groupPtr selGroup, nodePtr vEl)
		{
			nodePtr lcPort = lastConnectedGroupPort(selGroup).get();
			PointerHubConnect(backGroupPort(selGroup), vEl);
			nodePtr eventSource = sPack::selectHandler::eventSource / node();
			nodePtr eventType = sPack::selectHandler::eventType_illGExchenge / node();
			addEvent(nullptr, eventSource, eventType, lcPort);
			addEvent(nullptr, eventSource, eventType, vEl);
		}
		void deselect(groupPointer gi)
		{
			addEvent(nullptr, sPack::selectHandler::eventSource / node(),
				sPack::selectHandler::eventType_illGExchenge / node(), gi.get());
			disconnect(gi);
		}
		void delesectAll()
		{
			groupPtr selGroup = sPack::selectHandler::selectGroup / node();
			groupPointer gi(selGroup);
			do
			{
				if(gi.get().exist())
					deselect(gi);
			} while (gi.stepForward());
		}
		void changeSelection(nodePtr vEl)
		{
			groupPtr selGroup = sPack::selectHandler::selectGroup / node();
			groupPointer gi = groupOperations::getPointerToConnection(selGroup, vEl);
			if (gi.exist())
				deselect(gi);
			else
			{
				if (!shift())
					delesectAll();
				select(selGroup, vEl);
			}
		}
		
	};
}
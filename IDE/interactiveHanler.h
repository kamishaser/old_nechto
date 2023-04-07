#pragma once
#include "event.h"
#include "textOut.h"

namespace nechto::ide
{
	class interactiveHandler : public singleConnectedEntity
	{
	public:
		interactiveHandler()
		{
			eConnect(creator::createEntity(entityT::singleConnection));
			fabricate(sPack::interactiveHandler::getPlan(), node());
		}
		void finalConnect(nodePtr ideNode)
		{
			NumNumConnect(-sPack::nechtoIde::interactiveHandler / ideNode,
				node(), sPack::nechtoIde::selectHanlder.last(), 3);//подключение к системе
			nodePtr mouse = sPack::nechtoIde::mouse / ideNode;
			//подписки
			nodePtr mClickSubscription =
				sPack::interactiveHandler::mClickSubscription / node();
			subscribeToSource(
				mClickSubscription, sPack::mouse::eventSource / mouse);
			subscribeToType(
				mClickSubscription, sPack::mouse::eventType_ButtonClicked / mouse);
			nodePtr mOverSubscription =
				sPack::interactiveHandler::mOverChSubscription / node();
			subscribeToSource(
				mOverSubscription, sPack::mouse::eventSource / mouse);
			subscribeToType(
				mOverSubscription, sPack::mouse::eventType_OveredChanged / mouse);
			//подключение мыши
			NumHubConnect(-sPack::interactiveHandler::mouse / node(),
				mouse, sPack::interactiveHandler::mouse.last());
		}
		void update()
		{
			updatemOverChange();
			updatemClick();
		}
	private:
		void updatemOverChange()
		{
			nodePtr subscription = sPack::interactiveHandler::mOverChSubscription / node();
			for (nodePtr event; +(event = readEvent(subscription));)
			{
				if (+event)
				{
					nodePtr oldIEl = sPack::interactiveHandler::currentIEl / node();
					if (oldIEl.exist())
					{
						ielEvent(sPack::interactiveHandler::et_mOver);
						resetCurIEl();
					}
					nodePtr vEl = sPack::event::content / event;
					if (vEl.exist() && sPack::vNode::vBlockIsNode(vEl))
					{
						nodePtr nodeBoard = sPack::vNode::nodeBoard / vEl;
						bool atInterfaceBoard = reversePort(portPointer(nodeBoard, 3)).getLocalPos() == 1;
						nodePtr iEl = sPack::vNode::node / vEl;
						if (atInterfaceBoard && +iEl)
						{
							ielEvent(sPack::interactiveHandler::et_mOver);
							setCurIEl(iEl);
						}
					}
				}
			}
		}
		void updatemClick()
		{
			nodePtr mouse		= sPack::interactiveHandler::mouse / node();
			nodePtr posNode = sPack::mouse::velPos  / mouse;
			nodePtr lBNode	= sPack::mouse::leftButton / mouse;
			nodePtr rBNode	= sPack::mouse::rightButton / mouse;
			nodePtr mBNode	= sPack::mouse::middleButton / mouse;
			nodePtr curVel	= sPack::mouse::curVel / mouse;
			nodePtr subscription = sPack::interactiveHandler::mClickSubscription / node();
			for (nodePtr event; +(event = readEvent(subscription));)
			{
				nodePtr buttonNode = sPack::event::content / event;
				i64 buttonStatus = 0;
				buttonNode >> buttonStatus;
				buttonStatus = ((buttonStatus % 2) == 0);
				nodePtr iel = sPack::interactiveHandler::currentIEl / node();

				if (buttonStatus && +iel)
				{
					if (buttonNode == lBNode)
						ielEvent(sPack::interactiveHandler::et_lClick, posNode);
					else if (buttonNode == rBNode)
						ielEvent(sPack::interactiveHandler::et_rClick, posNode);
					else if (buttonNode == mBNode)
						ielEvent(sPack::interactiveHandler::et_mClick, posNode);
				}
			}
		}
		void ielEvent(path evTypePath, nodePtr content = nullptr)
		{
			nodePtr evSource = sPack::interactiveHandler::curIElEventSource / node();
			if (evSource.exist())
				addEvent(nullptr, evSource, evTypePath / node(), content);
		}
		void resetCurIEl()
		{
			disconnect("2"_np / node(), 1);
		}
		void setCurIEl(nodePtr iel)
		{
			NumNumConnect("2"_np / node(), "0"_np / iel, 1, 0);
		}
	};
}
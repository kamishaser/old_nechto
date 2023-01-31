#pragma once

#include "namedExCon.h"
#include "periodLimiter.h"

namespace nechto::ide
{
	class basicButton
	{
	protected:
		bool bClick = false; //раззовое событие нажатия кнопки 
		bool eClick = false; //раззовое событие отжатия кнопки
		bool status = false; //состояние кнопки
		milliseconds clickStartTime = 0ms;
	public:
		//разовое событие. Происходит после кратковременного нажатия
		bool bClickEvent()
		{
			if (!bClick)
				return false;
			bClick = false;
			return true;
		}
		bool eClickEvent()
		{
			if (!eClick)
				return false;
			eClick = false;
			return true;
		}
		bool isPressed() const
		{
			return status;
		}
		milliseconds pressTime() const
		{
			return currentTime() - clickStartTime;
		}
		
		//обновление кнопки
		bool update(bool newStatus)
		{
			if (newStatus)
			{
				if (!status)
				{
					status = true;
					bClick = true;
					clickStartTime = currentTime();
					return true;
				}
			}
			else if (status)
			{
				status = false;
				eClick = true;
			}
			return false;
		}
	};
	class sharedButton : public basicButton
	{
		using clickEvent = std::function<void(teptr<sharedButton>)>;
		//№0 - content
		//№3 - список кнопок в котором она находится
		clickEvent bcEvent;
	public:
		sharedButton(clickEvent bce = nullptr)
			:bcEvent(bce) {}
		nodePtr content(teptr<visualGroup> node)
		{
			return node.connection(0);
		}
		//обновление кнопки
		bool update(teptr<visualGroup> node, bool newStatus, teptr<visualNode> vNode)
		{
			if (basicButton::update(newStatus))
			{
				if (vNode.exist())
					NumHubConnect(node, vNode, 0);
				else
					nearestDisconnect(node, 0);
				return true;
			}
			return false;
		}
		bool update(teptr<visualGroup> node, bool newStatus)
		{
			if (basicButton::update(newStatus))
			{
				if (bcEvent)
					bcEvent(node);
				return true;
			}
			return false;
		}
		nodePtr getList(teptr<visualGroup> node)
		{
			nodePtr temp = node.connection(3);
			if (!temp.exist())
				return nullptr;
			return temp.connection(0);
		}
	};
	teptr<sharedButton> createButton(const std::wstring& name, clickEvent bce = nullptr)
	{
		return creator::createEntity(entityT::oneSideLink, new 
			oneSideLinkedEntity<sharedButton>(sharedButton(bce)));
	}
	using clickEvent = std::function<void(teptr<sharedButton>)>;
}

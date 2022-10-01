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
	class sharedButton : public namedExCon, public basicButton
	{
	public:
		sharedButton(std::wstring name)
			:namedExCon(name){}
		nodePtr content()
		{
			return getConnection(0);
		}
		//обновление кнопки
		bool update(bool newStatus, nodePtr v1 = nullNodePtr)
		{
			if (basicButton::update(newStatus))
			{
				if (v1.exist())
					NumHubConnect(get(), v1, 0);
				else
					numDisconnect(get(), 0);
				return true;
			}
			return false;
		}
	};
}

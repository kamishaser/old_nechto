#pragma once

#include "namedExCon.h"
#include "periodLimiter.h"

namespace nechto::ide
{
	class basicButton
	{
	protected:
		bool bClick = false; //�������� ������� ������� ������ 
		bool eClick = false; //�������� ������� ������� ������
		bool status = false; //��������� ������
		milliseconds clickStartTime = 0ms;
	public:
		//������� �������. ���������� ����� ���������������� �������
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
		
		//���������� ������
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
		//���������� ������
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

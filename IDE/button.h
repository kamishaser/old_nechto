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
		//№0 - content
		//№3 - список кнопок в котором она находится
	public:
		sharedButton(std::u16string name)
			:namedExCon(name){}
		sharedButton(nodePtr emptyExternalObject, std::u16string name)
			:namedExCon(emptyExternalObject, name) {}
		nodePtr content()
		{
			return getConnection(0);
		}
		//обновление кнопки
		bool update(bool newStatus, visualNode* vNode)
		{
			if (basicButton::update(newStatus))
			{
				if (vNode)
					NumHubConnect(get(), vNode->get(), 0);
				else
					numDisconnect(get(), 0);
				return true;
			}
			return false;
		}
		bool update(bool newStatus)
		{
			if (basicButton::update(newStatus))
			{
				return true;
			}
			return false;
		}
		static sharedButton* getByNode(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalObject)
				return nullptr;
			return dynamic_cast<sharedButton*>(v1->getData<externalObject*>());
		}
		nodePtr getList()
		{
			nodePtr temp = getConnection(3);
			if (!temp.exist())
				return nullNodePtr;
			return temp->connection[0];
		}
	};
	sharedButton* createButton(const std::u16string& name)
	{
		return new sharedButton(newExObjNode(0), name);
	}

}

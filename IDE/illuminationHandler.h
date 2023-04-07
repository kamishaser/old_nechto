#pragma once
#include "ideStructurePack.h"
#include "visual.h"
#include "event.h"

namespace nechto::ide
{
	class illuminationHandler : public singleConnectedEntity
	{
	public:
		illuminationHandler()
		{
			eConnect(creator::createEntity(entityT::singleConnection));
			fabricate(sPack::illuminationHandler::getPlan(), node());
		}
		void finalConnect(nodePtr ideNode)
		{
			NumNumConnect(-sPack::nechtoIde::illuminationHandler / ideNode,
				node(), sPack::nechtoIde::illuminationHandler.last(), 3);//подключение к системе
			nodePtr mouse = sPack::nechtoIde::mouse / ideNode;
			nodePtr mOverSubscription = 
				sPack::illuminationHandler::mOverSubcription / node();
			subscribeToSource(
				mOverSubscription, sPack::mouse::eventSource / mouse);
			subscribeToType(
				mOverSubscription, sPack::mouse::eventType_OveredChanged / mouse);
			nodePtr eventType = 
				sPack::illuminationHandler::eventType_illGExchenge / node();
			nodePtr illGExchangeSubscription = 
				sPack::illuminationHandler::illGExchengeSubscription / node();
			subscribeToType(
				illGExchangeSubscription, eventType);
		}
		void update()
		{
			updateMOver();
			updateIllGroupExchange();
		}
	private:
		void updateMOver()
		{
			nodePtr subscription = sPack::illuminationHandler::mOverSubcription / node();
			for (nodePtr event; +(event = readEvent(subscription));)
			{
				if (+event)
				{
					nodePtr oldMOver = sPack::illuminationHandler::mouseCurVel / node();
					nodePtr mOver = sPack::event::content / event;
					if (oldMOver.exist())
					{
						disconnect("00"_np / node(), 2);
						updateVelIllumination(oldMOver);
					}
					if (mOver.exist())
					{
						NumHubConnect("00"_np / node(), mOver, 2);
						updateVelIllumination(mOver);
					}
				}
			}
		}
		void updateIllGroupExchange()
		{
			nodePtr subscription = sPack::illuminationHandler::illGExchengeSubscription / node();
			groupPointer gi(sPack::eventSubscription::nonHandledEvent / subscription);
			do
			{
				nodePtr event = "3"_np / gi.get();
				if (+event)
				{
					nodePtr vEl = sPack::event::content / event;
					updateVelIllumination(vEl);
					moveToHandled(subscription, gi);
				}
			} while (gi.stepForward());
		}
		i64 findVElIlluminationNumber(nodePtr vel)
		{
			if (!vel.exist())
				return 0;
			if (vel == sPack::illuminationHandler::mouseCurVel / node())
				return col::Mouse;
			portPointer pi(vel);
			i64 max = 0;
			do
			{
				if (groupPtr::match(pi.get()))
				{
					i64 temp = compareGroup(pi.get(), vel);
					if (temp > max)
						max = temp;
				}
			} while (pi.stepForward());
			
			return max;
		}
		void updateVelIllumination(nodePtr vel)
		{
			sPack::vNode::drawableBlock / sPack::draBlock::illuminationColor / vel << 
				findVElIlluminationNumber(vel);
		}
		i64 compareGroup(groupPtr group, nodePtr vel)
		{
			if (group == sPack::illuminationHandler::errorGroup / node())
				return col::Error;
			if (group == sPack::illuminationHandler::warningGroup / node())
				return col::Error;
			if (group == sPack::illuminationHandler::tip0Group / node())
				return col::Tip0;
			if (group == sPack::illuminationHandler::tip1Group / node())
				return col::Tip1;
			if (group == sPack::illuminationHandler::tip2Group / node())
				return col::Tip2;
			groupPtr selGroup = sPack::illuminationHandler::selectGroup / node();
			if (group != selGroup)
				return 0;
			if (lastConnectedGroupPort(selGroup).get() == vel)
				return col::Sel1;
			//обработка номера selection
			return col::Sel3;
		}
	};
}
#pragma once
#include "ideStructurePack.h"
#include "drawableBlock.h"
#include "event.h"

namespace nechto::ide
{
	namespace button
	{
		const serialPlan getPlan()
		{
			return sPack::eventSubscription::getPlan();
		}
		nodePtr fabricate(nodePtr interactiveHandler, nodePtr vNode)
		{
			nodePtr iel = fabricate(sPack::interEl::getPlan(getPlan()));
			subscribeToSource(sPack::interEl::data / iel, sPack::interEl::eventSource / iel);
			subscribeToType(sPack::interEl::data / iel,
				sPack::interactiveHandler::et_lClick / interactiveHandler);
			NumNumConnect(vNode, iel, 0, 2);
			return iel;
		}
		bool isClicked(nodePtr iel)
		{
			return +readEvent(sPack::interEl::data / iel);
		}
	};
}
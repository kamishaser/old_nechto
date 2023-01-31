#pragma once
#include "connecter.h"
#include "hubManager.h"

namespace nechto
{
	portPointer firstEmptyHubPort(existing<nodePtr> eptr)
	{
		assert(eptr.type() != nodeT::Hub);
		portPointer ci(eptr);
		while (true)
		{
			if (!ci.hub.hub().exist())
				hubManager::insertHub(ci, 1);
			ci.goToNextHub();
			for (int i = 0; i < 4; ++i)
				if (!ci.hub.connection(i).exist())
				{
					ci.setLocalPos(i);
					return ci;
				}
		}
	}
	groupPointer firstEmptyGroupPort(groupPtr group)
	{
		groupPointer gi(group);
		while (true)
		{
			for (int i = 0; i < 4; ++i)
			{
				gi.setLocalPos(i);
				if (!gi.get().exist())
					return gi;
			}
			if (gi.hub.hub() == group.firstGroupHub())
			{
				hubManager::insertHub(gi, 1);
			}
			gi.goToNextHub();
		}
	}
	portPointer lastConnectedPort(existing<nodePtr> node)
	{
		portPointer ptr(node);
		portPointer lastConnected(nullptr, hubPosPair());
		do
		{
			if (ptr.get().exist())
				lastConnected = ptr;
		} while (ptr.stepForward());
		return lastConnected;
	}
	groupPointer lastConnectedGroupPort(groupPtr group)
	{
		groupPointer gi(group);
		gi.stepBack();
		do
		{
			if (gi.get().exist())
				return gi;
		} while (gi.stepBack());
		return groupPointer(group, hubPosPair());
	}
	groupPointer backGroupPort(groupPtr group)
	{
		groupPointer gi(group);
		gi.stepBack();

		if (gi.get().exist())//если последний порт занят
		{
			hubManager::insertHub(gi, 1);
			gi.goToNextHub();
			gi.setLocalPos(0);
			return gi;
		}
		gi.stepBackToNextConnected();
		gi.stepForward();
		assert(!gi.get().exist());
		return gi;
	}
	/*возврощает итератор на ближайший порт ноды node подключённый к connection.
	* Ели не находит - возвращает пустой итератор */
	portPointer findNearestNonGroupConnection(
		existing<nodePtr> node, existing<nodePtr> connection)
	{
		portPointer ci(node);
		do
		{
			if (ci.get() == connection)
				return ci;
		} while (ci.stepForward());
		return portPointer(nullptr, hubPosPair(nullptr, 0));
	}
	/*возврощает итератор на ближайший порт группы group подключённый к connection.
	* Ели не находит - возвращает пустой итератор */
	groupPointer findNearestGroupConnection(
		groupPtr group, existing<nodePtr> connection)
	{
		groupPointer gi(group);
		do
		{
			if (gi.get() == connection)
				return gi;
		} while (gi.stepForward());
		return groupPointer(nullptr, hubPosPair(nullptr, 0));
	}
	pointer findNearestConnection(
		existing<nodePtr> node, existing<nodePtr> connection)
	{
		if (node.type() == nodeT::Group)
		{
			auto gi = findNearestGroupConnection(groupPtr(node), connection);
			if (gi.exist())
				return gi;
		}
		auto ci = findNearestNonGroupConnection(node, connection);
		return ci;
	}
	bool hasConnection(existing<nodePtr> node0, existing<nodePtr> node1)
	{
		portPointer p1(node0);
		do
		{
			if (p1.get() == node1)
				return true;
		} while (p1.stepForward());
		return false;
	}
}
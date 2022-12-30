#pragma once
#include "connecter.h"
#include "hubManager.h"

namespace nechto
{
	portIterator firstEmptyHubPort(existing<nodePtr> eptr)
	{
		assert(eptr.type() != nodeT::Hub);
		portIterator ci(eptr);
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
	groupIterator firstEmptyGroupPort(groupPtr group)
	{
		groupIterator gi(group);
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
	portIterator lastConnectedPort(existing<nodePtr> node)
	{
		portIterator iter(node);
		portIterator lastConnected(nullptr, hubPosPair());
		do
		{
			if (iter.get().exist())
				lastConnected = iter;
		} while (iter.stepForward());
		return lastConnected;
	}
	groupIterator lastConnectedGroupPort(groupPtr group)
	{
		groupIterator gi(group);
		gi.stepBack();
		do
		{
			if (gi.get().exist())
				return gi;
		} while (gi.stepBack());
		return groupIterator(group, hubPosPair());
	}
	groupIterator backGroupPort(groupPtr group)
	{
		groupIterator gi(group);
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
	portIterator findNearestNonGroupConnection(
		existing<nodePtr> node, existing<nodePtr> connection)
	{
		portIterator ci(node);
		do
		{
			if (ci.get() == connection)
				return ci;
		} while (ci.stepForward());
		return portIterator(nullptr, hubPosPair(nullptr, 0));
	}
	/*возврощает итератор на ближайший порт группы group подключённый к connection.
	* Ели не находит - возвращает пустой итератор */
	groupIterator findNearestGroupConnection(
		groupPtr group, existing<nodePtr> connection)
	{
		groupIterator gi(group);
		do
		{
			if (gi.get() == connection)
				return gi;
		} while (gi.stepForward());
		return groupIterator(nullptr, hubPosPair(nullptr, 0));
	}
	iterator findNearestConnection(
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
		portIterator p1(node0);
		do
		{
			if (p1.get() == node1)
				return true;
		} while (p1.stepForward());
		return false;
	}
}
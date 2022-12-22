#pragma once
#include "connectionIterator.h"
#include "groupPtr.h"

#include "creator.h"
#include "connecter.h"
#include "hubManager.h"

namespace nechto::groupOperations
{
	i64 numberOfMembers(groupPtr group)
	{
		i64 counter = 0;
		groupIterator i(group);
		do
		{
			if (i.get().exist())
				++counter;
		} while (i.stepForward());
		return counter;
	}
	bool empty(groupPtr group)
	{
		groupIterator i(group);
		do
		{
			if (i.get().exist())
				return false;
		} while (i.stepForward());
		return true;
	}

	
	void nearestGroupDisconnectAll(groupPtr group)
	{
		groupIterator gi(group);
		do
		{
			if (gi.get().exist())
				nearestDisconnect(gi);
		} while (gi.stepForward());
	}
	void clear(groupPtr group)
	{
		nearestGroupDisconnectAll(group);
		groupIterator gi(group);
		gi.goToNextHub();
		hubManager::eraseHub(gi, -1);
	}
	bool contains(groupPtr group, nodePtr v1)
	{
		groupIterator gi(group);
		do
		{
			if (gi.get() == v1)
				return true;
		} while (gi.stepForward());
		return false;
	}
	groupIterator getIteratorToConnection(groupPtr group, nodePtr v1)
	{
		groupIterator gi(group);
		do
		{
			if (gi.get() == v1)
				return gi;
		} while (gi.stepForward());
		return nullGroupIterator;
	}
}

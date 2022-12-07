#pragma once
#include "connectionIterator.h"
#include "groupPtr.h"

#include "creator.h"
#include "connecter.h"
#include "hubEraser.h"

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
		gi.stepForward();
		hubEraser::eraseHubChain(gi);
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
	void hubOrderedList(std::vector<hubPtr>& hubSet, groupPtr group, 
		const groupIterator& begin)
	{
		groupIterator gi(group);
		while (true)
		{
			if (gi.getHPPair().hub == begin)
				break;
			if (!gi.nextHub())
				return;
		} 
		do
		{
			hubSet.push_back(existing<nodePtr>(gi.getHPPair().hub));
		} while (gi.nextHub());
	}
	void orderedIteratorList(std::vector<groupIteratorPtr>& iterSet, 
		groupPtr group, const existing<groupIterator>& begin)
	{
		std::vector<hubPtr> hubSet;
		hubOrderedList(hubSet, group, begin.getHPPair().hub);
		auto nearestIterator = iterSet.begin();
		if(nearestIterator->getHPPair().hub == begin.getHPPair().hub)
			while (nearestIterator->getHPPair().getPos() < begin.getHPPair().getPos())
				++nearestIterator;

		for (int i1 = 0; i1 < iterSet.size(); ++i1)
		{
			for (int i2 = i1; i2 < iterSet.size() ++i2)
			{
			}
		}
	}

	void compress(const groupIterator& begin)
	{

		groupIterator pullIter = begin;
		groupIterator pushIter = begin;
		std::vector<groupIteratorPtr> iterSet;
		hubEraser::getAllGroupIterators(iterSet, begin.getPurpose());
		do
		{
			if (pullIter.get().exist())
			{
				connecter::swap(pullIter, pushIter);
				pushIter.stepForward();
			}
		} while (pullIter.stepForward());
		if (pushIter.pos() != 0)
			pushIter.nextHub();
		hubEraser::eraseHubChain(pushIter);
	}
	void compress(groupPtr group)
	{
		compress(groupIterator(group));
	}
}

#pragma once
#include "connectionIterator.h"
#include "groupPtr.h"

#include "creator.h"
#include "connecter.h"

namespace nechto::group
{
	//void reset(nodePtr v1)
	//{
	//	groupIterator i1(v1);
	//	while (true)
	//	{
	//		nodePtr connection = i1.oneSideDisconnect();
	//		if (connection.exist())
	//		{
	//			//при удалении ноды надо удалить все существующие соединения
	//			if (typeSubtypeCompare(
	//				connection, node::Pointer, pointer::GroupIter) &&
	//				(connection->connection[0] == v1))
	//			{
	//				connection->connection[0] = nullNodePtr;
	//				connection->setData<pointer::hubPosPair>(
	//					pointer::hubPosPair(nullNodePtr, 0));

	//			}
	//			else
	//				oneSideDisconnect(connection, v1);
	//		}
	//		if (i1.pos() == 3)
	//		{
	//			nodePtr vTemp = i1.currentHub;
	//			bool end = !i1.GoToNextHub();
	//			nodeStorage::terminal.deallocate(vTemp);
	//			i1.setLocalPos(0);
	//			if (end)
	//			{
	//				assert(typeCompare(i1.currentHub, node::Deleted));
	//				return;
	//			}
	//		}
	//		else
	//			i1.stepForward();
	//	}
	//}
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

	void clear(groupPtr v1)
	{
		reset(v1);
		initializeEmpty(v1);
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
		return groupIterator(nullNodePtr, pointer::hubPosPair(nullNodePtr, 0));
	}
	bool check(groupPtr v1)
	{
		assert(v1->getType() == node::Group);
		if (v1->data < 1)
			return false;
		//nodePtr hubIter = v1->connection[0];

		//if (!v1->getType())
		return true;
	}

	void compress(const groupIterator& begin)
	{
		groupIterator pullIter = begin;
		groupIterator pushIter = begin;
		do
		{
			if (pullIter.get().exist())
			{
				connecter::swap(pullIter, pushIter);
				pushIter.stepForward();
			}
		} while (pullIter.stepForward());
		if (pushIter.pos() != 0)
			pushIter.stepForward();
		while (pushIter.currentHub != pushIter.mainNode->connection[0])
		{
			assert(hub::empty(pushIter.currentHub));
			pushIter.eraseHub();
		}
	}
	void compress(groupPtr group)
	{
		compress(groupIterator(group));
	}
}

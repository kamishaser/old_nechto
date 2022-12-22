#pragma once
#include "typeDeclarations.h"
#include "nodePtr.h"
#include "pointerPtr.h"
#include "connectionIterator.h"

#include "creator.h"

namespace nechto
{
	class connecter
	{
	public:
		static bool connect(existing<iterator> iter1, existing<iterator> iter2)
		{
			if ((iter1.get().exist()) || (iter2.get().exist()))
				return false;
			if(iter1.inGroup())
				if(groupConnectProhibition(groupIterator(iter1), iter2))
					return false;
			if (iter2.inGroup())
				if (groupConnectProhibition(groupIterator(iter2), iter1))
					return false;
			iter1.getHPPair().hub.node()->port[iter1.getLocalPos()] = iter2.getPurpose();
			iter2.getHPPair().hub.node()->port[iter2.getLocalPos()] = iter1.getPurpose();
			return true;
		}
		static bool disconnect(existing<iterator> iter1, existing<iterator> iter2)
		{
			if (!isConnection(iter1, iter2))
				return false;//только двустороннии соединения
			resetIterator(iter1);
			resetIterator(iter2);
			iter1.getHPPair().hub.node()->port[iter1.getLocalPos()] = nullptr;
			iter2.getHPPair().hub.node()->port[iter2.getLocalPos()] = nullptr;
			return true;
		}
		static bool isConnection(existing<iterator> iter1, existing<iterator> iter2)
		{
			return (iter1.get() == iter2.getPurpose()) &&
				(iter2.get() == iter1.getPurpose());
		}
		static void swap(existing<iterator> iter1, existing<iterator> iter2)
		{
			assert(iter1.getPurpose() == iter2.getPurpose());
			resetIterator(iter1);
			resetIterator(iter2);
			nodePtr temp = iter1.get();
			iter1.getHPPair().hub.node()->port[iter1.getLocalPos()] = iter2.get();
			iter2.getHPPair().hub.node()->port[iter2.getLocalPos()] = temp;
		}
		
		//исключение хаба из цепочки без оповещения итераторов
		
		static void resetIterator(existing<iterator> iter)
		{
			existing<nodePtr> node = iter.getHPPair().hub;
			if (node.type() == nodeT::Pointer &&
				(iter.getLocalPos() == 0) && (node.subtype() > 0))
				pointerPtr(node).setHPPair(hubPosPair(nullptr, 0));
		}
		static bool groupConnectProhibition(groupIterator gi, iterator iter)
		{
			if (iter.hub.type() == nodeT::Pointer)
				if (iter.getGlobalPos() == 0)
					return true;
			return false;
		}
	};
	iterator findNearestConnection(
		existing<nodePtr> node, existing<nodePtr> connection);//определено в portSearch
	portIterator firstEmptyHubPort(existing<nodePtr> eptr);
	void nearestDisconnect(existing<iterator> i1)
	{
		if (!i1.getPurpose().exist())
			return;
		connecter::disconnect(i1, findNearestConnection(i1.get(), i1.getPurpose()));
	}
	void nearestDisconnect(existing<nodePtr> node1, char port)
	{
		nearestDisconnect(portIterator(node1, port));
	}
	void nearestDisconnect(existing<nodePtr> node1, existing<nodePtr> node2)
	{
		connecter::disconnect(
			findNearestConnection(node1, node2), findNearestConnection(node2, node1));
	}

	void simplifiedNumNumConnect(
		existing<nodePtr> node1, existing<nodePtr> node2, char port1, char port2)
	{
		portIterator ci1(node1, port1);
		portIterator ci2(node2, port2);
		if (ci1.get().exist())
			nearestDisconnect(ci1);
		if (ci2.get().exist())
			nearestDisconnect(ci2);
		connecter::connect(ci1, ci2);
	}
	void simplifiedNumHubConnect(
		existing<nodePtr> node1, existing<nodePtr> node2, char port1)
	{
		portIterator ci1(node1, port1);
		if (ci1.get().exist())
			nearestDisconnect(ci1);
		connecter::connect(ci1, firstEmptyHubPort(node2));
	}
	void simplifiedHubHubConnect(
		existing<nodePtr> node1, existing<nodePtr> node2, char port1)
	{
		connecter::connect(
			firstEmptyHubPort(node1), firstEmptyHubPort(node2));
	}
	void simplifiedIterIterConnect(iterator ci1, iterator ci2)
	{
		if (ci1.get().exist())
			nearestDisconnect(ci1);
		if (ci2.get().exist())
			nearestDisconnect(ci2);
		connecter::connect(ci1, ci2);
	}
	void simplifiedIterNumConnect(
		iterator ci1, existing<nodePtr> node2, char port2)
	{
		assert(port2 < 4);
		portIterator ci2(node2, port2);
		if (ci1.get().exist())
			nearestDisconnect(ci1);
		if (ci2.get().exist())
			nearestDisconnect(ci2);
		connecter::connect(ci1, ci2);
	}
	void simplifiedIterHubConnect(
		iterator ci1, existing<nodePtr> node2)
	{
		if (ci1.get().exist())
			nearestDisconnect(ci1);
		connecter::connect(ci1, firstEmptyHubPort(node2));
	}
	void creator::disconnectAll(existing<nodePtr> node)
	{
		portIterator iter(node);
		do
		{
			nearestDisconnect(iter);
		} while (iter.stepForward());
	}
	void creator::disconnectAllGroup(groupPtr group)
	{
		groupIterator iter(group);
		do
		{
			nearestDisconnect(iter);
		} while (iter.stepForward());
	}
}
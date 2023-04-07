#pragma once
#include "typeDeclarations.h"
#include "nodePtr.h"
#include "pointerPtr.h"
#include "connectionPointer.h"

#include "creator.h"

namespace nechto
{
	class connecter
	{
	public:
		static bool connect(existing<pointer> ptr1, existing<pointer> ptr2)
		{
			//когда будешь делать защиту операций, не забудь про swap
			if (ptr1.get().exist())
				disconnect(ptr1);
			if (ptr2.get().exist())
				disconnect(ptr2);
			if(ptr1.inGroup())
				if(groupConnectProhibition(groupPointer(ptr1), ptr2))
					return false;
			if (ptr2.inGroup())
				if (groupConnectProhibition(groupPointer(ptr2), ptr1))
					return false;
			ui32 lp1 = ptr1.getLocalPos();
			ui32 lp2 = ptr2.getLocalPos();
			nodeData* h1 = ptr1.hub.node();
			nodeData* h2 = ptr2.hub.node();

			h1->port[lp1] = ptr2.getPurpose();
			h1->reverseAddress[lp1] = ptr2.hub;
			h1->reversePosition.setLocalPos(lp1, lp2);

			h2->port[lp2] = ptr1.getPurpose();
			h2->reverseAddress[lp2] = ptr1.hub;
			h2->reversePosition.setLocalPos(lp2, lp1);
			
			
			/*ptr1.getHPPair().hub.node()->port[ptr1.getLocalPos()] = ptr2.getPurpose();
			ptr1.getHPPair().hub.node()->backPosition[ptr1.getLocalPos()] = ptr2.getConPos();
			ptr2.getHPPair().hub.node()->port[ptr2.getLocalPos()] = ptr1.getPurpose();
			ptr2.getHPPair().hub.node()->backPosition[ptr2.getLocalPos()] = ptr1.getConPos();*/
			return true;
		}
		static bool disconnect(existing<pointer> ptr1)
		{
			if (!ptr1.get().exist())
				return false;
			existing<pointer> ptr2 = reversePort(ptr1);
			resetPointer(ptr1);
			resetPointer(ptr2);
			ptr1.getHPPair().hub.node()->port[ptr1.getLocalPos()] = nullptr;
			ptr2.getHPPair().hub.node()->port[ptr2.getLocalPos()] = nullptr;
			return true;
		}
		static bool isConnection(existing<pointer> ptr1, existing<pointer> ptr2)
		{
			return (ptr1.get() == ptr2.getPurpose()) &&
				(ptr2.get() == ptr1.getPurpose());
		}
		static void swap(existing<pointer> ptr1, existing<pointer> ptr2)
		{
			pointer revPort1 = reversePort(ptr1);
			pointer revPort2 = reversePort(ptr2);

			disconnect(ptr1);
			disconnect(ptr2);

			connect(ptr1, revPort2);
			connect(ptr2, revPort1);
		}
	public:
		
		//исключение хаба из цепочки без оповещения итераторов
		
		static void resetPointer(existing<pointer> ptr)
		{
			existing<nodePtr> node = ptr.hub;
			if (node.type() == nodeT::Pointer &&
				(ptr.getLocalPos() == 0) && (node.subtype() > 0))
				pointerPtr(node).setHPPair(hubPosPair(nullptr, 0));
		}
		static bool groupConnectProhibition(groupPointer gi, pointer ptr)
		{
			if (ptr.hub.type() == nodeT::Pointer)
				if (ptr.getGlobalPos() == 0)
					return true;
			return false;
		}
	};
	pointer findNearestConnection(
		existing<nodePtr> node, existing<nodePtr> connection);//определено в portSearch
	portPointer firstEmptyHubPort(existing<nodePtr> eptr);
	void disconnect(existing<pointer> i1)
	{
		if (!(i1.getPurpose().exist() && i1.get().exist()))
			return;
		connecter::disconnect(i1);
	}
	void disconnect(existing<nodePtr> node1, char port)
	{
		disconnect(portPointer(node1, port));
	}
	void disconnect(nodePtr node1, char port)
	{
		if (node1.exist())
			disconnect(existing<nodePtr>(node1), port);
	}
	void disconnect(existing<nodePtr> node1, existing<nodePtr> node2)
	{
		pointer ptr1 = findNearestConnection(node1, node2);
		if(ptr1.exist())
			connecter::disconnect(ptr1);
	}

	void NumNumConnect(
		existing<nodePtr> node1, existing<nodePtr> node2, char port1, char port2)
	{
		portPointer ci1(node1, port1);
		portPointer ci2(node2, port2);
		if (ci1.get().exist())
			disconnect(ci1);
		if (ci2.get().exist())
			disconnect(ci2);
		connecter::connect(ci1, ci2);
	}
	void NumHubConnect(
		existing<nodePtr> node1, existing<nodePtr> node2, char port1)
	{
		portPointer ci1(node1, port1);
		if (ci1.get().exist())
			disconnect(ci1);
		connecter::connect(ci1, firstEmptyHubPort(node2));
	}
	void HubHubConnect(
		existing<nodePtr> node1, existing<nodePtr> node2)
	{
		connecter::connect(
			firstEmptyHubPort(node1), firstEmptyHubPort(node2));
	}
	void PointerPointerConnect(pointer ci1, pointer ci2)
	{
		if (ci1.get().exist())
			disconnect(ci1);
		if (ci2.get().exist())
			disconnect(ci2);
		connecter::connect(ci1, ci2);
	}
	void PointerNumConnect(
		pointer ci1, existing<nodePtr> node2, char port2)
	{
		assert(port2 < 4);
		portPointer ci2(node2, port2);
		if (ci1.get().exist())
			disconnect(ci1);
		if (ci2.get().exist())
			disconnect(ci2);
		connecter::connect(ci1, ci2);
	}
	void PointerHubConnect(
		pointer ci1, existing<nodePtr> node2)
	{
		if (ci1.get().exist())
			disconnect(ci1);
		connecter::connect(ci1, firstEmptyHubPort(node2));
	}
	void creator::disconnectAll(existing<nodePtr> node)
	{
		portPointer ptr(node);
		do
		{
			disconnect(ptr);
		} while (ptr.stepForward());
	}
	void creator::disconnectAllGroup(groupPtr group)
	{
		groupPointer ptr(group);
		do
		{
			disconnect(ptr);
		} while (ptr.stepForward());
	}
}
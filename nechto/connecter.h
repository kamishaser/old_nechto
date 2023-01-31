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
			if ((ptr1.get().exist()) || (ptr2.get().exist()))
				return false;
			if(ptr1.inGroup())
				if(groupConnectProhibition(groupPointer(ptr1), ptr2))
					return false;
			if (ptr2.inGroup())
				if (groupConnectProhibition(groupPointer(ptr2), ptr1))
					return false;
			ptr1.getHPPair().hub.node()->port[ptr1.getLocalPos()] = ptr2.getPurpose();
			ptr2.getHPPair().hub.node()->port[ptr2.getLocalPos()] = ptr1.getPurpose();
			return true;
		}
		static bool disconnect(existing<pointer> ptr1, existing<pointer> ptr2)
		{
			if (!isConnection(ptr1, ptr2))
				return false;//только двустороннии соединения
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
			assert(ptr1.getPurpose() == ptr2.getPurpose());
			resetPointer(ptr1);
			resetPointer(ptr2);
			nodePtr temp = ptr1.get();
			ptr1.getHPPair().hub.node()->port[ptr1.getLocalPos()] = ptr2.get();
			ptr2.getHPPair().hub.node()->port[ptr2.getLocalPos()] = temp;
		}
		
		//исключение хаба из цепочки без оповещения итераторов
		
		static void resetPointer(existing<pointer> ptr)
		{
			existing<nodePtr> node = ptr.getHPPair().hub;
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
	void nearestDisconnect(existing<pointer> i1)
	{
		if (!(i1.getPurpose().exist() && i1.get().exist()))
			return;
		connecter::disconnect(i1, findNearestConnection(i1.get(), i1.getPurpose()));
	}
	void nearestDisconnect(existing<nodePtr> node1, char port)
	{
		nearestDisconnect(portPointer(node1, port));
	}
	void nearestDisconnect(existing<nodePtr> node1, existing<nodePtr> node2)
	{
		pointer ptr1 = findNearestConnection(node1, node2);
		pointer ptr2 = findNearestConnection(node2, node1);
		if(ptr1.exist() && ptr2.exist())
			connecter::disconnect(ptr1, ptr2);
	}

	void NumNumConnect(
		existing<nodePtr> node1, existing<nodePtr> node2, char port1, char port2)
	{
		portPointer ci1(node1, port1);
		portPointer ci2(node2, port2);
		if (ci1.get().exist())
			nearestDisconnect(ci1);
		if (ci2.get().exist())
			nearestDisconnect(ci2);
		connecter::connect(ci1, ci2);
	}
	void NumHubConnect(
		existing<nodePtr> node1, existing<nodePtr> node2, char port1)
	{
		portPointer ci1(node1, port1);
		if (ci1.get().exist())
			nearestDisconnect(ci1);
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
			nearestDisconnect(ci1);
		if (ci2.get().exist())
			nearestDisconnect(ci2);
		connecter::connect(ci1, ci2);
	}
	void PointerNumConnect(
		pointer ci1, existing<nodePtr> node2, char port2)
	{
		assert(port2 < 4);
		portPointer ci2(node2, port2);
		if (ci1.get().exist())
			nearestDisconnect(ci1);
		if (ci2.get().exist())
			nearestDisconnect(ci2);
		connecter::connect(ci1, ci2);
	}
	void PointerHubConnect(
		pointer ci1, existing<nodePtr> node2)
	{
		if (ci1.get().exist())
			nearestDisconnect(ci1);
		connecter::connect(ci1, firstEmptyHubPort(node2));
	}
	void creator::disconnectAll(existing<nodePtr> node)
	{
		portPointer ptr(node);
		do
		{
			nearestDisconnect(ptr);
		} while (ptr.stepForward());
	}
	void creator::disconnectAllGroup(groupPtr group)
	{
		groupPointer ptr(group);
		do
		{
			nearestDisconnect(ptr);
		} while (ptr.stepForward());
	}
}
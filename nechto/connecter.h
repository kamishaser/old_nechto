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
			iter1.getHPPair().hub.node()->port[iter1.pos()] = iter2.getPurpose();
			iter2.getHPPair().hub.node()->port[iter2.pos()] = iter1.getPurpose();
		}
		static bool disconnect(existing<iterator> iter1, existing<iterator> iter2)
		{
			if (!isConnection(iter1, iter2))
				return false;//только двустороннии соединения
			resetIterator(iter1);
			resetIterator(iter2);
			iter1.getHPPair().hub.node()->port[iter1.pos()] = nullptr;
			iter2.getHPPair().hub.node()->port[iter2.pos()] = nullptr;
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
			iter1.getHPPair().hub.node()->port[iter1.pos()] = iter2.get();
			iter2.getHPPair().hub.node()->port[iter2.pos()] = temp;
		}
		//вставляет hub после iter
		static void insertHub(existing<iterator> iter, hubPtr hub)
		{
			//hub нигде не задействован
			assert(!hub.previous().exist() && !hub.hub().exist());
			nodePtr previous = iter.getHPPair().hub;
			nodePtr next = previous.hub();
			previous.node()->hubPort = hub;
			hub.setPrevious(previous);
			if (!next.exist())//если хаб последний в цепочке
				return;
			hub.node()->hubPort = next;
			hubPtr(next).setPrevious(hub);
		}
		//исключение хаба из цепочки без оповещения итераторов
		
		static void resetIterator(existing<iterator> iter)
		{
			existing<nodePtr> node = iter.getHPPair().hub;
			if (node.type() == nodeT::Pointer &&
				(iter.pos() == 0) && (node.subtype() > 0))
				pointerPtr(node).setHPPair(hubPosPair(nullptr, 0));
		}
	};
	portIterator firstEmptyHubPort(existing<nodePtr> eptr)
	{
		assert(eptr.type() != nodeT::Hub);
		portIterator ci(eptr);
		while (true)
		{
			nodePtr nextHub = ci.get().hub();
			if (!nextHub.exist())
			{
				nextHub = creator::createHub();
				connecter::insertHub(ci, hubPtr(nextHub));
			}
			ci.setHPPair(hubPosPair(nextHub, 0));
			for (int i = 0; i < 4; ++i)
				if (!nextHub.connection(i).exist())
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
			nodePtr nextHub = gi.get().hub();
			if (!nextHub.exist())
			{
				nextHub = creator::createHub();
				connecter::insertHub(gi, hubPtr(nextHub));
			}
			gi.setHPPair(hubPosPair(nextHub, 0));
		}
	}
	portIterator lastConnectedPort(existing<nodePtr> node)
	{
		portIterator iter(node);
		portIterator lastConnected(node, hubPosPair());
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
			hubPtr nextHub = creator::createHub();
			connecter::insertHub(gi, nextHub);
			gi.setHPPair(hubPosPair(nextHub, 0));
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
			if(gi.exist())
				return gi;
		}
		auto ci = findNearestNonGroupConnection(node, connection);
		return ci;
	}
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
	void simplifiedIterIterConnect(portIterator ci1, portIterator ci2)
	{
		if (ci1.get().exist())
			nearestDisconnect(ci1);
		if (ci2.get().exist())
			nearestDisconnect(ci2);
		connecter::connect(ci1, ci2);
	}
	void simplifiedIterNumConnect(
		portIterator ci1, existing<nodePtr> node2, char port2)
	{
		portIterator ci2(node2, port2);
		if (ci1.get().exist())
			nearestDisconnect(ci1);
		if (ci2.get().exist())
			nearestDisconnect(ci2);
		connecter::connect(ci1, ci2);
	}
	void simplifiedIterHubConnect(
		portIterator ci1, existing<nodePtr> node2)
	{
		if (ci1.get().exist())
			nearestDisconnect(ci1);
		connecter::connect(ci1, firstEmptyHubPort(node2));
	}
}
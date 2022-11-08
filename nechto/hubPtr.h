#pragma once
#include "nodePtr.h"
namespace nechto
{
	//хаб каждый хаб обязательно содержит следующие данные:
		//hubConnection: следующий хаб
		//data: предыдущий хаб
	class connecter;
	class hubPtr : public existing<nodePtr>
	{
		friend class connecter;
		void setPrevious(existing<nodePtr> previous)
		{
			setData<nodePtr>(previous);
		}
	public:
		hubPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		existing<nodePtr> previous()
		{
			return existing<nodePtr>(getData<nodePtr>());
		}
		bool empty(nodePtr v1)
		{
			if (v1->hasConnection(0) || v1->hasConnection(1) ||
				v1->hasConnection(2) || v1->hasConnection(3))
				return false;
			return true;
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::Hub;
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
	};


	namespace hub
	{
		////создаёт новый хаб и вставляет в цепочку после текущего
		//inline nodePtr insert(nodePtr previous, nodePtr mainNode)
		//{

		//	nodePtr hub = newNode(node::Hub);
		//	hub->setData(std::pair<nodePtr, nodePtr>(previous, mainNode));
		//	
		//	nodePtr next = previous->hubConnection;
		//	previous->hubConnection = hub;
		//	if (next.exist())
		//	{
		//		hub->hubConnection = next;
		//		next->setData(std::pair<nodePtr, nodePtr>(hub, mainNode));
		//	}
		//	return hub;
		//}
		//inline nodePtr insertBack(nodePtr chainEnd, nodePtr mainNode)
		//{
		//	assert(!chainEnd->hasHub());
		//	nodePtr hub = newNode(node::Hub);
		//	hub->setData(std::pair<nodePtr, nodePtr>(chainEnd, mainNode));
		//	chainEnd->hubConnection = hub;
		//	return hub;
		//}
		//const nodePtr getMain(const nodePtr hub)
		//{
		//	assert(hub->getType() == node::Hub);
		//	return hub->getData<std::pair<nodePtr, nodePtr>>().second;
		//}
		//
		////следующая нода в цепочке
		//nodePtr next(nodePtr v1)
		//{
		//	nodePtr nextH = v1->hubConnection;
		//	if (nextH.exist())
		//		return nextH;
		//	else
		//		return v1->getData<std::pair<nodePtr, nodePtr>>().second;
		//}
		////предыдущая нода в цепочке

		////проверяет, пустой ли хаб
		
		////удаляет все соединения хаба
		//void deleteAllConnectionsInHub(nodePtr v1, nodePtr mainNode)
		//{
		//	for (int i = 0; i < 4; ++i)
		//	{
		//		nodePtr con = v1->connection[i].exchange(nullNodePtr);
		//		if (con.exist())
		//			nechto::oneSideDisconnect(con, mainNode);
		//	}
		//}
		////удаляет хаб
		//void deleteHub(nodePtr v1, nodePtr mainNode)
		//{
		//	assert(v1->getType() == node::Hub);
		//	deleteAllConnectionsInHub(v1, mainNode);
		//	nodeStorage::terminal.deallocate(v1);
		//}
		////исключает хаб из цепочки. При наличии соединений в хабе 2-сторонних соединений отключает
		//void erase(nodePtr v1, nodePtr mainNode)
		//{
		//	assert(typeCompare(v1, node::Hub));
		//	nodePtr prev = previous(v1);
		//	nodePtr next = v1->hubConnection;
		//	prev->hubConnection = next;
		//	if (next.exist())
		//		next->setData(std::pair<nodePtr, nodePtr>(prev, mainNode));
		//	deleteHub(v1, mainNode);
		//}
		//bool checkChain(nodePtr v1)
		//{
		//	nodePtr hubIter = v1;
		//	while (true)
		//	{
		//		nodePtr next = hubIter->hubConnection;
		//		if (!next.exist())
		//			return true;
		//		if (next->getType() != node::Hub)
		//			return false;
		//		auto data = next->getData<std::pair<nodePtr, nodePtr>>();
		//		if (data.first != hubIter || data.second != v1)
		//			return false;
		//		hubIter = next;
		//		
		//	}
		//}
		//void compress(nodePtr v1)
		//{
		//	assert(v1.exist());
		//	assert(typeCompare(v1, node::Group));
		//	connectionIterator pullIter(v1);
		//	connectionIterator pushIter(v1);
		//	if (!v1->hasHub())
		//		return;
		//	pullIter.GoToNextHub();
		//	pushIter.GoToNextHub();
		//	do
		//	{
		//		nodePtr temp = *pullIter;
		//		*pullIter = nullNodePtr;//надо переделать для оптимизации
		//		if (temp.exist())
		//		{
		//			*pushIter = temp;
		//			pushIter.stepForward();
		//		}
		//	} while (pullIter.stepForward());
		//	if (pushIter.pos() != 0)
		//		pushIter.stepForward();
		//	while (pushIter.currentHub != v1->connection[0])
		//	{
		//		assert(hub::empty(pushIter.currentHub));
		//		pushIter.eraseHub();
		//	}
		//}

	}
}
#pragma once
#include "node.h"
namespace nechto
{
	const nodePtr newNode();
	void disconnect(nodePtr v1, nodePtr v2);
	
	//хаб каждый хаб обязательно содержит следующие данные:
		//hubConnection: следующий хаб
		//data: пара адресов: предыдущий хаб и основная нода.
	namespace hub
	{
		//создаёт новый хаб и вставляет в цепочку после текущего
		inline nodePtr insert(nodePtr previous, nodePtr mainNode)
		{

			nodePtr hub = newNode(node::Hub);
			hub->setData(std::pair<nodePtr, nodePtr>(previous, mainNode));
			
			nodePtr next = previous->hubConnection;
			previous->hubConnection = hub;
			if (next.exist())
			{
				hub->hubConnection = next;
				next->setData(std::pair<nodePtr, nodePtr>(hub, mainNode));
			}
			return hub;
		}
		inline nodePtr pushBack(nodePtr chainEnd, nodePtr mainNode)
		{
			assert(!chainEnd->hasHub());
			nodePtr hub = newNode(node::Hub);
			hub->setData(std::pair<nodePtr, nodePtr>(chainEnd, mainNode));
			chainEnd->hubConnection = hub;
			return hub
		}
		const nodePtr getMain(const nodePtr hub)
		{
			assert(hub->getType() == node::Hub);
			return hub->getData<std::pair<nodePtr, nodePtr>>().second;
		}
		//конец цепочки хабов (!внимание! не работает с массивами)
		nodePtr chainEnd(nodePtr v1)
		{
			nodePtr iter = v1;
			nodePtr next = iter->hubConnection;
			while (next.exist())
			{
				nodePtr iter = next;
				nodePtr next = iter->hubConnection;
			}
			return iter;
		}
		//следующая нода в цепочке
		nodePtr next(nodePtr v1)
		{
			nodePtr nextH = v1->hubConnection;
			if (nextH.exist())
				return nextH;
			else
				return v1->getData<std::pair<nodePtr, nodePtr>>().second;
		}
		//предыдущая нода в цепочке
		nodePtr previous(nodePtr v1)
		{
			if (v1->getType() == node::Hub)
				return v1->getData<std::pair<nodePtr, nodePtr>>().first;
			//если v1 не хаб, то она не содержит данных о последнем хабе
			//придётся скакать в конец цепочки
			return chainEnd(v1);
		}
		//проверяет, пустой ли хаб
		bool empty(nodePtr v1)
		{
			if (v1->hasConnection(0) || v1->hasConnection(1) ||
				v1->hasConnection(2) || v1->hasConnection(3))
				return false;
			return true;
		}
		//исключает хаб из цепочки. При наличии соединений в хабе 2-сторонне отключает
		void erase(nodePtr v1, nodePtr mainNode)
		{
			assert(typeCompare(v1, node::Hub));
			nodePtr previous = previous(v1);
			nodePtr next = v1->hubConnection;
			for (int i = 0; i < 4; ++i)
			{
				nodePtr con = v1->connection[i].exchange(nullNodePtr);
				if(con.exist())
					nechto::oneSideDisconnect(con, mainNode);
			}
			previous->hubConnection = next;
			if (next.exist())
				next->setData(std::pair<nodePtr, nodePtr>(previous, mainNode));
			deleteNode(v1);
		}
		void deleteChain(nodePtr v1)
		{

		}

		/*void fill(nodePtr v1, char subtype);
		void reset(nodePtr v1);
		void perform(nodePtr v1);
		bool check(nodePtr v1);
		void copy(nodePtr v1);*/

	}
}
#pragma once
#include "node.h"

namespace nechto
{
	namespace pointer
	{
		bool check(nodePtr v1)
		{
			assert(v1->getType() == node::Pointer);
			char subtype = v1->getSubtype();
			if (subtype == pointer::Reference)
				return true;
			nodePtr currentHub = v1->connection[0];
			nodePtr mainNode = v1->connection[1];
			if (!currentHub.exist() || !mainNode.exist())
				return false;
			if (currentHub->getData<std::pair<nodePtr, nodePtr>>().second != mainNode)
				return false;
			if (subtype == pointer::ConIter)
			{
				
			}
			else
			{
				if (mainNode->getType() != node::Array)
					return false;
				if (currentHub->getType() != node::Hub)
					return false;
			}
			return true;
		}
		
		

		//что хранится в итераторе:
		//в поле данных номер элемента
		//к нулевому текущий хаб(внимание: односторонее соединение!!!)
		//к первому соединению подключен массив(в случае coniter основная нода)
		
		//присваивание значения ноде того же типа
		void assigment(nodePtr v0, const nodePtr v1)
		{
			assert(v1.exist());
			assert(v1->getType() == node::Pointer);
			char subtype = v1->getSubtype();
			if (!subtype)//reference
			{
				NumHubConnect(v0, v1->connection[0], 0);
				v0->setData<i64>(0);
			}
			else
			{
				NumConnect(v0, v1->connection[0], 0);
				NumHubConnect(v0, v1->connection[1], 1);
				v0->setData<i64>(v1->getData<i64>());
			}
		}
		
		nodePtr follow(const nodePtr v1)
		{
			assert(v1.exist());
			assert(v1->getType() == node::Pointer);
			if (!v1->getSubtype())//reference
			{
				return v1->connection[0].load();
			}
			else
			{
				nodePtr hub = v1->connection[0];
				if (!hub.exist())
					return nullNodePtr;
				return hub->connection[v1->getData<i64>() & 3ll];
			}
		}

		void initializeEmpty(nodePtr v1)
		{
			v1->setData<char>(0);
		}
		
	}

}
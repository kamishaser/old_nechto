#pragma once
#include "node.h"
#include "hub.h"

namespace nechto
{
	namespace pointer
	{
		bool check(nodePtr v1)
		{
			assert(v1->getType() == node::Pointer);
			char subtype = v1->getSubtype();
			if (subtype == pointer::Simple)
				return true;
			auto hpp = v1->getData<hubPosPair>();
			nodePtr mainNode = v1->connection[0];
			nodePtr currentHub = hpp.first;
			if (!currentHub.exist() || !mainNode.exist())
				return false;
			if (currentHub->getData<std::pair<nodePtr, nodePtr>>().second != mainNode)
				return false;
			if (subtype == pointer::ConIter)
			{
				if (currentHub != mainNode)//подключено к хабу
				{
					if (currentHub->getType() != node::Hub)
						return false;
					if (hub::getMain(currentHub) != mainNode)
						return false;
				}
			}
			else
			{
				if (mainNode->getType() != node::Group)
					return false;
				if (currentHub->getType() != node::Hub)
					return false;
				if (hub::getMain(currentHub) != mainNode)
					return false;
			}
			return true;
		}
		
		
		
		//что хранится в итераторе:
		//в поле данных хранится пара из указателя на хаб и номера позиции в нём
		//к нулевому соединению подключен массив(в случае coniter основная нода)
		
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
				v0->setData<hubPosPair>(v1->getData<hubPosPair>());
			}
		}
		bool set(nodePtr pointer, nodePtr v1);//определено в connectionIterator.h
	}

}
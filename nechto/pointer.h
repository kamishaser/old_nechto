#pragma once
#include "node.h"

namespace nechto
{
	void deleteNode(nodePtr v1);
	namespace pointer
	{
		void reset(nodePtr v1);
		void perform(nodePtr v1);
		bool check(nodePtr v1);
		

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
				v1->setData<i64>(0);
			}
			else
			{
				NumConnect(v0, v1->connection[0], 0);
				NumHubConnect(v0, v1->connection[1], 1);
				v0->setData<i64>(v1->getData<i64>());
			}
		}
		//инициализирует ссылку или итератор
		void initialize(nodePtr v1, const nodePtr sourse, bool atBegin = true)
		{
			assert(sourse.exist());
			char subtype = v1->getSubtype();
			if (!subtype)//reference
			{
				NumHubConnect(v1, sourse, 0);
			}
			else if (subtype == pointer::ConIter)
			{
				NumHubConnect(v1, sourse, 1);
				if (atBegin)
				{
					v1->setData<i64>(0);
					NumConnect(v1, sourse, 0);
				}
				else
				{//так как номер последнего хаба нигде не записан приходится перебирать
					nodePtr i = sourse;//итератор перехода по хабам
					int64_t position = 3;
					while (true)
					{
						nodePtr hub = i->hubConnection();
						if (!hub.exist())
							break;
						position += 4;
						i = i->hubConnection();
					}
					NumConnect(v1, i, 0);
					v1->setData<i64>(position);
				}
			}
			else//ArrayIter
			{
				if (atBegin)
					assigment(v1, sourse->connection[0]);
				else
					assigment(v1, sourse->connection[1]);
			}

			assert(v1->getSubtype() == sourse->getSubtype())
			char sourseType = sourse->getType();
			if (sourseType == node::Hub)//нельзя подключаться к хабам
				return false;
			else 
		}
		nodePtr follow(nodePtr v1)
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
	}

}
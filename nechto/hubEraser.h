#pragma once
#include "connecter.h"
#include "creator.h"

#include "hubPtr.h"
#include "groupPtr.h"
#include "connectionIterator.h"

namespace nechto
{
	//набор функци€ дл€ удалени€ хабов
	class hubEraser
	{
	public:
		//удаление хаба из основной цепочки
		static bool eraseHub(existing<portIterator> iter)
		{
			if (iter.getHPPair().hub.type() != nodeT::Hub)
				return false;
			hubPtr hub(existing<nodePtr>(iter.getHPPair().hub));
			nodePtr previous = hub.previous();//предыдущий хаб
			nodePtr next = hub.hub();//следующий хаб
			std::vector<portIteratorPtr> iterSet;//список всех итераторов на сию ноду
			getAllPortIterators(iterSet, iter.getPurpose());
			if (!erase(iterSet, iter))
				return false;
			if (!next.exist())
				previous.node()->hubPort = nullptr;
			hubPtr(next).connect(previous);
			return true;
		}
		//удаление хаба из группы
		static bool eraseHub(existing<groupIterator> iter)
		{
			hubPtr hub(existing<nodePtr>(iter.getHPPair().hub));
			nodePtr previous = hub.previous();//предыдущий хаб
			hubPtr next = existing<nodePtr>(hub.hub());//следующий хаб
			std::vector<groupIteratorPtr> iterSet;//список всех итераторов на сию ноду

			getAllGroupIterators(iterSet, iter.getPurpose());
			if (!erase(iterSet, iter))
				return false;
			next.connect(previous);
			return true;
		}
		
		//вырезать цепь хабов
		static bool eraseHubChain(existing<portIterator> iter, 
			portIterator end = nullPortIterator)
		{
			if (iter.getHPPair().hub.type() != nodeT::Hub)
				return false;
			hubPtr hub(existing<nodePtr>(iter.getHPPair().hub));
			nodePtr previous = hub.previous();//предыдущий хаб
			nodePtr next = hub.hub();//следующий хаб
			std::vector<portIteratorPtr> iterSet;//список всех итераторов на сию ноду
			getAllPortIterators(iterSet, iter.getPurpose());
			bool success = true;
			while (true)
			{
				if (!erase(iterSet, iter))
				{
					success = false;
					break;
				}
				
				if (!next.exist())//если удалЄн последний хаб в цепочке
					break;
				if (hub == end.getHPPair().hub)//хаб совпадает с end
					break;
				hub = hubPtr(existing<nodePtr>(next));
				next = hub.hub();
			}
			if (success)
			{
				if (!next.exist())
					previous.node()->hubPort = nullptr;
				hubPtr(next).connect(previous);
			}
			else
			{
				hub.connect(previous);
			}
			return success;
		}
		static bool eraseHubChain(existing<groupIterator> iter,
			groupIterator end = nullGroupIterator)
		{
			hubPtr hub(existing<nodePtr>(iter.getHPPair().hub));
			nodePtr previous = hub.previous();//предыдущий хаб
			hubPtr next = existing<nodePtr>(hub.hub());//следующий хаб
			groupPtr group = existing<nodePtr>(iter.getPurpose());

			std::vector<groupIteratorPtr> iterSet;//список всех итераторов на сию ноду
			getAllGroupIterators(iterSet, iter.getPurpose());
			bool success = true;
			while (true)
			{
				if (!erase(iterSet, iter))
				{
					success = false;
					break;
				}

				if (next == group.firstGroupHub())//если next - перва€ нода
					break;
				if (hub == end.getHPPair().hub)//хаб совпадает с end
					break;
				hub = hubPtr(existing<nodePtr>(next));
				next = existing<nodePtr>(hub.hub());
			}
			if (success)
				next.connect(previous);
			else
				hub.connect(previous);
			return success;
		}


		//получить все итераторы, указывающие на ноду
		static void getAllPortIterators(std::vector<portIteratorPtr>& iteratorSet,
				existing<nodePtr> node)
		{
			portIterator iter(node);
			do
			{//если к сему порту подключен итератор указывающий на эту ноду
				if ((portIteratorPtr::match(iter.get())) &&
					(iter.get().connection(0) == node))
					iteratorSet.push_back(
						iteratorPtr(pointerPtr(existing<nodePtr>(iter.get()))));

			} while (iter.stepForward());
		}
		static void getAllGroupIterators(std::vector<groupIteratorPtr>& iteratorSet,
			existing<nodePtr> node)
		{
			portIterator iter(node);
			do
			{//если к сему порту подключен итератор указывающий на эту ноду
				if ((groupIteratorPtr::match(iter.get())) &&
					(iter.get().connection(0) == node))
					iteratorSet.push_back(
						iteratorPtr(pointerPtr(existing<nodePtr>(iter.get()))));

			} while (iter.stepForward());
		}
	private:
		static bool erase(std::vector<portIteratorPtr>& iterSet,
			existing<portIterator> iter)
		{
			if (!hubPtr::match(existing<nodePtr>(iter.getHPPair().hub)))
				return false; // удал€ть можно только существующие хабы
			hubPtr hub(existing<nodePtr>(iter.getHPPair().hub));
			//нельз€ удал€ть хаб, не удалив предварительно все его подлкючени€
			if (!hub.empty())
				return false;
			for (int i = 0; i < iterSet.size(); ++i)
			{
				if (iterSet[i].getHPPair().hub == iter.getHPPair().hub)
				{
					iterSet[i].setHPPair(hubPosPair());//сброс
					iterSet[i] = iterSet.back();//удаление из списка
					iterSet.pop_back();
				}
			}
			creator::deleteHub(hub);
			return true;
		}
		static bool erase(std::vector<groupIteratorPtr>& iterSet,
			existing<groupIterator> iter)
		{
			hubPtr hub(existing<nodePtr>(iter.getHPPair().hub));
			groupPtr group = existing<nodePtr>(iter.getPurpose());
			//нельз€ удал€ть хаб, не удалив предварительно все его подлкючени€
			if (!hub.empty())
				return false;
			if (hub == group.firstGroupHub())
				return false;//нельз€ удал€ть первый хаб в группе
			for (int i = 0; i < iterSet.size(); ++i)
			{
				if (iterSet[i].getHPPair().hub == iter.getHPPair().hub)
				{
					iterSet[i].setHPPair(hubPosPair());//сброс
					iterSet[i] = iterSet.back();//удаление из списка
					iterSet.pop_back();
				}
			}
			creator::deleteHub(hub);//итоговое удаление хаба
			return true;
		}
	};
	
}

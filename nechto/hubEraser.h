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
		bool eraseHub(existing<portIterator> iter)
		{
			if (!hubPtr::match(existing<nodePtr>(iter.getHPPair().hub)))
				return false; // удал€ть можно только существующие хабы
			hubPtr hub(existing<nodePtr>(iter.getHPPair().hub));
			//нельз€ удал€ть хаб, не удалив предварительно все его подлкючени€
			if (!hub.empty())
				return false;

			nodePtr previous = hub.previous();//предыдущий хаб
			std::vector<iteratorPtr> iterSet;//список всех итераторов на сию ноду

			getAllPortIterators(iterSet, iter.getPurpose());
			for (auto& i : iterSet)
				resetIterator(i, iter);//сброс всех итераторов на сей хаб
			nodePtr next = hub.hub();//следующий хаб
			//замыкание цепочки с исключением удал€емого хаба
			previous.node()->hubPort = next;
			if (next.exist())
				hubPtr(next).setPrevious(previous);
			creator::deleteHub(hub);
			return true;
		}
		//удаление хаба из группы
		bool eraseHub(existing<groupIterator> iter)
		{
			hubPtr hub(existing<nodePtr>(iter.getHPPair().hub));
			groupPtr group = existing<nodePtr>(iter.getPurpose());
			//нельз€ удал€ть хаб, не удалив предварительно все его подлкючени€
			if (!hub.empty())
				return false;

			nodePtr previous = hub.previous();//предыдущий хаб
			if (previous == hub)//нельз€ удал€ть последний хаб в группе
				return false;
			std::vector<iteratorPtr> iterSet;//список всех итераторов на сию ноду

			getAllPortIterators(iterSet, iter.getPurpose());
			for (auto& i : iterSet)
				resetIterator(i, iter);//сброс всех итераторов на сей хаб
			hubPtr next = existing<nodePtr>(hub.hub());//следующий хаб
			if (hub == group.firstGroupHub())//если удал€емый хаб €вл€етс€ первым
				group.setFirstGroupHub(next);

			//замыкание цепочки с исключением удал€емого хаба
			if (next.exist())
				hubPtr(next).connect(previous);
			else
				previous.node()->hubPort = nullptr;
			creator::deleteHub(hub);//итоговое удаление хаба
			return true;
		}
		
		//вырезать цепь хабов
		//bool eraseHubChain(portIterator begin, portIterator end)
		//{
		//	if (!hubPtr::match(existing<nodePtr>(begin.getHPPair().hub)))
		//		return false; // удал€ть можно только существующие хабы

		//	//если begin и end указывают на разные ноды, выкинуть ошибку
		//	if (begin.getPurpose() != end.getPurpose())
		//		return false;
		//	
		//	nodePtr previous = hubPtr(existing<nodePtr>(begin.getHPPair().hub)).previous();//предыдущий хаб
		//	std::vector<iteratorPtr> iterSet;//список всех итераторов на сию ноду
		//	getAllPortIterators(iterSet, begin.getPurpose());

		//	while (true)
		//	{
		//		hubPtr hub(existing<nodePtr>(begin.getHPPair().hub));
		//		if (!hub.empty())
		//		{//если в хабе есть подключени€ или на хаб указывает итератор end
		//			//замкнуть previous и 
		//			previous.node()->hubPort = hub.hub();
		//			if (hub.hub())
		//				hubPtr(hub.hub()).setPrevious(previous);
		//			return true;
		//		}
		//		if (hub == end.getHPPair().hub)
		//		{//если в хабе есть подключени€ или на хаб указывает итератор end
		//		}
		//	}
		//}
		//bool eraseHubChain(groupIterator begin, groupIterator end);


		//получить все итераторы, указывающие на ноду
		static std::vector<iteratorPtr>
			getAllPortIterators(std::vector<iteratorPtr>& iteratorSet,
				existing<nodePtr> node)
		{
			portIterator iter(node);
			do
			{
				if (iteratorPtr::match(iter.get()))
					if (iter.get().connection(0) == node)
						iteratorSet.push_back(
							pointerPtr(existing<nodePtr>(iter.get())));

			} while (iter.stepForward());
			return iteratorSet;
		}
		static void resetIterator(iteratorPtr& nodeIter, iterator iter)
		{
			if (nodeIter.getHPPair().hub == iter.getHPPair().hub)
				nodeIter.setHPPair(hubPosPair());
		}
	private:
		static void erase(std::vector<iteratorPtr>& iteratorSet,
			existing<portIterator> iter)
		{
			
		}

	};
	
}

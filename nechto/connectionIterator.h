#pragma once
#include "pointerPtr.h"
#include "hubPtr.h"
#include "groupPtr.h"

namespace nechto
{
	struct iterator
	{
	protected:
		hubPosPair hpp;
		nodePtr purpose;

		bool exist()
		{//публичное использование сего имени опасно путсницей с get().exist()
			return connected();
		}
	public:
		iterator() {}
		iterator(nodePtr Purpose, hubPosPair HPP)
			:purpose(Purpose), hpp(HPP) {}
		explicit iterator(iteratorPtr iterPtr)
			:purpose(iterPtr.purpose()), hpp(iterPtr.getHPPair()) {}
		bool connected()const
		{
			return hpp.exist();
		}
		operator bool()const
		{return connected(); }
		nodePtr get()const 
		{
			return hpp.follow();
		}
		void setLocalPos(char npos)
		{
			hpp.pos = npos;
		}

		//позиция итератора в хабе. Номер соединения от 0 до 3
		char pos() const
		{
			return hpp.pos & static_cast<char>(3);
		}
		//вытянуть данные из ноды итератора
		void pull(iteratorPtr iter)
		{
			auto hpp = iter.getHPPair();
			purpose = iter.purpose();
			hpp = iter.getHPPair();
		}
		//отправить данные в ноду итератор
		bool push(iteratorPtr iter) const
		{
			if (iter.purpose() != purpose)
				return false;
			iter.setHPPair(hpp);
		}
		bool operator==(const iterator& iter) const
		{
			return (hpp == iter.hpp) && (purpose == iter.purpose);
		}
		bool operator!=(const iterator& iter) const
		{
			return (hpp != iter.hpp) || (purpose != iter.purpose);
		}
		nodePtr getPurpose() const
		{
			return purpose;
		}
		hubPosPair getHPPair() const
		{
			return hpp;
		}
		void setHPPair(hubPosPair HPP)
		{
			hpp = HPP;
		}
	};

	class portIterator : public iterator
	{
	public:
		explicit portIterator(existing<nodePtr> Purpose, char pos = 0)
			:iterator(Purpose, hubPosPair(Purpose, 0)) {}
		explicit portIterator(portIteratorPtr conIter)
			:iterator(iteratorPtr(conIter)){}
		portIterator(nodePtr Purpose, hubPosPair hpp)
			:iterator(Purpose, hpp) {}
		bool nextHub()
		{
			nodePtr nextNode = hpp.hub.hub();
			//если следующего хаба не существует - переход в основную ноду
			if (nextNode.exist())
			{
				hpp.hub = nextNode;
				return true;
			}
			else
			{
				hpp.hub = purpose;
				return false;
			}
		}
		bool stepForward()
		{
			++hpp.pos;
			if (hpp.pos & 3ll)
				return true;
			else
				return nextHub();
		}
		//итератор на первом элементе
		bool atFirst()
		{
			if (!pos())
				return false;
			if (hpp.hub != purpose)
				return false;
			return true;
		}
		//итератор на последнем элементе
		bool atLast()
		{
			if (pos() != 3)
				return false;
			if (hpp.hub.hub().exist())
				return false;
			return true;
		}

		
		//static portIterator backEmptyHubPort(existing<nodePtr> eptr)
		bool stepForwardToNextConnected()
		{
			do
			{
				if (!stepForward())
					return false;
			} while (get().exist());
			return true;
		}


		////вставить хаб после текущего
		//nodePtr insertHub()
		//{
		//	return hub::insert(currentHub, mainNode);
		//}
		////удалить текущий хаб. Перевести все указывающие на него итераторы на следующий
		//void eraseHub()
		//{
		//	assert(currentHub != mainNode);
		//	//перебирает все подключённые ноды в поисках итератора, указывающего на удаляемый хаб
		//	portIterator conIter(mainNode);
		//	do
		//	{
		//		for (int i = 0; i < 4; ++i)
		//		{
		//			nodePtr temp = conIter.get();
		//			if (temp.exist())
		//				if (typeSubtypeCompare(temp, node::Pointer, pointer::ConIter))
		//				{//переводит на следующий
		//					nodePtr hub = temp->connection[0];
		//					if (hub == currentHub)
		//						temp->connection[0] = hub::next(hub);
		//				}
		//		}
		//	} while (conIter.stepForward());
		//	nodePtr next = hub::next(currentHub);
		//	hub::erase(currentHub, mainNode);
		//	currentHub = next;
		//}
	};
	class groupIterator : public iterator
	{
	public:
		explicit groupIterator(groupPtr group)
			:iterator(group, hubPosPair(group.firstGroupHub(), 0)){}
		explicit groupIterator(groupIteratorPtr groupIter)
			:iterator(iteratorPtr(groupIter)){}
		groupIterator(nodePtr Purpose, hubPosPair hpp)
			:iterator(Purpose, hpp) {}

		groupPtr group()
		{
			return groupPtr(purpose);
		}
		
		bool nextHub()
		{
			hpp.hub = hpp.hub.hub();
			return !atFirstHub();
		}
		bool previousHub()
		{
			bool result = hpp.hub != firstHub();
			hpp.hub = hubPtr(hpp.hub).previous();
			return result;
		}
		bool stepForward()
		{
			++hpp.pos;
			if (hpp.pos & 3ll)//если позиция == 0, переход в следующий хаб
				return true;
			else
				return nextHub();
				
		}
		bool stepForwardToNextConnected()
		{
			do
			{
				if (!stepForward())
					return false;
			} while (get().exist());
			return true;
		}
		bool stepBack()
		{
			--hpp.pos;
			if ((hpp.pos & 3ll) != 3)
				return true;//если перемещение происходит в границах одного хаба - всё норм
			else
				return previousHub();
		}
		bool stepBackToNextConnected()
		{
			do
			{
				if (!stepBack())
					return false;
			} while (get().exist());
			return true;
		}
		nodePtr firstHub()
		{
			return group().firstGroupHub();
		}

		bool atFirstHub()
		{
			return hpp.hub == group().firstGroupHub();
		}
		bool atFirst()
		{
			if (!(hpp.pos & 3ll))
				return false;
			return atFirstHub();
		}
		//nodePtr insertHub()
		//{
		//	return hub::insert(currentHub, mainNode);
		//}
		//void eraseHub()
		//{
		//	assert(currentHub != mainNode);
		//	if (mainNode->data.fetch_add(-1) == 1)
		//	{//нельзя удалять последний хаб
		//		mainNode->data.fetch_add(1);
		//		return;
		//	}
		//	//перебирает все подключённые ноды в поисках итератора, указывающего на удаляемый хаб
		//	portIterator conIter(mainNode);
		//	do
		//	{
		//		for (int i = 0; i < 4; ++i)
		//		{
		//			nodePtr temp = conIter.get();
		//			if (temp.exist())
		//				if (typeSubtypeCompare(temp, node::Pointer, pointer::GroupIter))
		//				{//переводит на следующий
		//					nodePtr hub = temp->connection[0];
		//					if (hub == currentHub)
		//						temp->connection[0] = hub::next(hub);
		//				}
		//		}
		//	} while (conIter.stepForward());
		//	if (atFirstHub())
		//		mainNode->setData<nodePtr>(hub::next(currentHub));
		//	nodePtr next = hub::next(currentHub);
		//	hub::erase(currentHub, mainNode);
		//	currentHub = next;
		//}
		///*вставляет элемент. 
		//При необходимости сдвигает следующие элементы вперёд на 1 хаб*/
		//bool insert(hubIterator backConnection)
		//{
		//	bool newHub = false;
		//	if (get().exist())
		//	{
		//		insertHub();
		//		newHub = true;
		//		nodePtr nextHub = hub::next(currentHub);
		//		for (int i = pos(); i < 4; ++i)
		//		{
		//			nextHub->connection[i] = currentHub->connection[i].load();
		//			currentHub->connection[i] = nullNodePtr;
		//		}
		//		IterIterConnect(*this, backConnection);
		//		stepForward();
		//	}
		//	else
		//	{
		//		IterIterConnect(*this, backConnection);
		//		if (pos() != 3)
		//			++position;
		//		else
		//		{
		//			nodePtr next = hub::next(currentHub);
		//			if (next == firstHub())
		//			{
		//				next = insertHub();
		//				newHub = true;
		//			}
		//			currentHub = next;

		//			setLocalPos(0);
		//			////////////////////////////////////////////////////////////недоделал
		//		}
		//	}
		//	return newHub;
		//}
	};
	/*bool pointer::set(nodePtr pointer, nodePtr v1)
	{
		if (pointer->getSubtype() == pointer::Simple)
		{
			if (v1.exist())
				NumHubConnect(pointer, v1, 0);
			else
				numDisconnect(pointer, 0);
		}
		else
		{
			hubIterator i0(pointer->connection[0],
				pointer->getData<pointer::hubPosPair>());
			if (v1.exist())
				i0.set(v1);
			else
				i0.oneSideDisconnect();
		}
		return true;
	}*/
}
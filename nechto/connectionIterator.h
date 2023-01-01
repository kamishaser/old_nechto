#pragma once
#include "iteratorPtr.h"
#include "hubPtr.h"
#include "groupPtr.h"

namespace nechto
{
	struct iterator : public hubPosPair
	{
	protected:
		nodePtr purpose;
	public:
		iterator() {}
		iterator(nodePtr Purpose, hubPosPair hpp)
			:hubPosPair(hpp), purpose(Purpose) {}
		explicit iterator(iteratorPtr iterPtr)
			:hubPosPair(iterPtr.getHPPair()), purpose(iterPtr.purpose()){}
		operator bool()const
		{return exist(); }
		nodePtr get()const 
		{
			return follow();
		}
		//вытянуть данные из ноды итератора
		void pull(iteratorPtr iter)
		{
			setHPPair(iter.getHPPair());
			purpose = iter.purpose();
		}
		//отправить данные в ноду итератор
		bool push(iteratorPtr iter) const
		{
			if (iter.purpose() != purpose)
				return false;
			iter.setHPPair(*this);
		}
		//bool operator==(const iterator& iter) const
		//{
		//	return (hpp == iter.hpp) && (purpose == iter.purpose);
		//}
		//bool operator!=(const iterator& iter) const
		//{
		//	return (hpp != iter.hpp) || (purpose != iter.purpose);
		//}
		nodePtr getPurpose() const
		{
			return purpose;
		}
		const hubPosPair& getHPPair() const
		{
			return *this;
		}
		void setHPPair(const hubPosPair& hpp)
		{
			hub = hpp.hub;
			position = hpp.position;
		}
	};

	class portIterator : public iterator
	{
	public:
		explicit portIterator(existing<nodePtr> Purpose, char pos = 0)
			:iterator(Purpose, hubPosPair(Purpose, pos)) {}
		explicit portIterator(portIteratorPtr conIter)
			:iterator(iteratorPtr(conIter)){}
		portIterator(nodePtr Purpose, hubPosPair hpp)
			:iterator(Purpose, hpp) {}
	private:
		bool nextHub()
		{
			nodePtr nextNode = hub.hub();
			//если следующего хаба не существует - переход в основную ноду
			if (nextNode.exist())
			{
				hub = nextNode;
				return true;
			}
			else
			{
				hub = purpose;
				setHubNumber(0);
				return false;
			}
		}
	public:
		bool goToNextHub()
		{
			shift(4);
			return nextHub();
		}
		bool stepForward()
		{
			if (++*this)
				return nextHub();
			else
				return true;
		}
		//итератор на первом элементе
		bool atFirst()
		{
			return getGlobalPos() == 0;
		}
		//итератор на последнем элементе
		bool atLast()
		{
			if (getLocalPos() != 3)
				return false;
			if (hub.hub().exist())
				return false;
			return true;
		}
		auto operator<=>(const portIterator& iter)
		{
			assert(hub == iter.hub);
			return getGlobalPos() <=> iter.getGlobalPos();
		}
	};
	class groupIterator : public iterator
	{
	public:
		explicit groupIterator(groupPtr group)
			:iterator(group, hubPosPair(group.firstGroupHub(), 0)){}
		explicit groupIterator(groupIteratorPtr groupIter)
			:iterator(iteratorPtr(groupIter)){}
		explicit groupIterator(iterator iter)
			:iterator(iter) 
		{
			if (iter.exist())
			{
				assert(iter.getPurpose().type() == nodeT::Group);
				assert(inGroup());
			}
		}
		groupIterator(nodePtr Purpose, hubPosPair hpp)
			:iterator(Purpose, hpp) {}

		groupPtr group()
		{
			return groupPtr(purpose);
		}
	private:
		bool nextHub()
		{
			hub = hub.hub();
			if (atFirstHub())
			{
				setHubNumber(0);
				return false;
			}
			else
				return true;
		}
		bool previousHub()
		{
			bool result = hub != firstHub();
			hub = hubPtr(hub).previous();
			if (!result)
				setHubNumber(groupPtr(getPurpose()).getNumberOfHubs() - 1);
			return result;
		}
	public:
		bool goToNextHub()
		{
			shift(4);
			return nextHub();
		}
		bool goToPreviousHub()
		{
			shift(-4);
			return previousHub();
		}
		bool stepForward()
		{
			if (++*this)
				return nextHub();
			return true;				
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
			if (--*this)
				return previousHub();
			return true;
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
			return hub == group().firstGroupHub();
		}
		bool atFirst()
		{
			if (getLocalPos())
				return false;
			return atFirstHub();
		}
		auto operator<=>(const groupIterator& iter)
		{
			assert(hub == iter.hub);
			return getGlobalPos() <=> iter.getGlobalPos();
		}
	};
	const portIterator nullPortIterator = portIterator(nullptr, hubPosPair());
	const groupIterator nullGroupIterator = groupIterator(nullptr, hubPosPair());
}
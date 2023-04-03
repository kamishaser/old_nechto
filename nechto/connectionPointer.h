#pragma once
#include "pointerPtr.h"
#include "hubPtr.h"
#include "groupPtr.h"

namespace nechto
{
	struct pointer : public hubPosPair
	{
	protected:
		nodePtr purpose;
	public:
		pointer() {}
		pointer(nodePtr Purpose, hubPosPair hpp)
			:hubPosPair(hpp), purpose(Purpose) {}
		explicit pointer(pointerPtr ptrPtr)
			:hubPosPair(ptrPtr.getHPPair()), purpose(ptrPtr.purpose()){}
		operator bool()const
		{return exist(); }
		nodePtr get()const 
		{
			return follow();
		}
		//вытянуть данные из ноды итератора
		void pull(pointerPtr ptr)
		{
			setHPPair(ptr.getHPPair());
			purpose = ptr.purpose();
		}
		//отправить данные в ноду итератор
		bool push(pointerPtr ptr) const
		{
			if (ptr.purpose() != purpose)
				return false;
			ptr.setHPPair(*this);
		}
		//bool operator==(const pointer& ptr) const
		//{
		//	return (hpp == ptr.hpp) && (purpose == ptr.purpose);
		//}
		//bool operator!=(const pointer& ptr) const
		//{
		//	return (hpp != ptr.hpp) || (purpose != ptr.purpose);
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

	class portPointer : public pointer
	{
	public:
		explicit portPointer(existing<nodePtr> Purpose, char pos = 0)
			:pointer(Purpose, hubPosPair(Purpose, pos)) {}
		explicit portPointer(portPointerPtr conPointer)
			:pointer(pointerPtr(conPointer)){}
		portPointer(nodePtr Purpose, hubPosPair hpp)
			:pointer(Purpose, hpp) {}
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
		auto operator<=>(const portPointer& ptr)
		{
			assert(hub == ptr.hub);
			return getGlobalPos() <=> ptr.getGlobalPos();
		}
	};
	class groupPointer : public pointer
	{
	public:
		explicit groupPointer(groupPtr group)
			:pointer(group, hubPosPair(group.firstGroupHub(), 0)){}
		explicit groupPointer(groupPointerPtr groupPointer)
			:pointer(pointerPtr(groupPointer)){}
		explicit groupPointer(pointer ptr)
			:pointer(ptr) 
		{
			if (ptr.exist())
			{
				assert(ptr.getPurpose().type() == nodeT::Group);
				assert(inGroup());
			}
		}
		
		groupPointer(nodePtr Purpose, hubPosPair hpp)
			:pointer(Purpose, hpp) {}
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
				{
					return false;
				}
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
		auto operator<=>(const groupPointer& ptr)
		{
			assert(hub == ptr.hub);
			return getGlobalPos() <=> ptr.getGlobalPos();
		}
	};
	const portPointer nullPortPointer = portPointer(nullptr, hubPosPair());
	const groupPointer nullGroupPointer = groupPointer(nullptr, hubPosPair());

	pointer reversePort(pointer p1)
	{
		if (!p1.get().exist())
			return pointer();
		ui32 locPos = p1.getLocalPos();
		ui32 revLocPos = p1.hub.reverseLocalPos(locPos);
		nodePtr reversehub = p1.hub.reverseAddress(locPos);
		if (reversehub.type() == nodeT::Hub)
			return pointer(p1.getPurpose(), hubPosPair(reversehub, revLocPos));
		return pointer(p1.getPurpose(), hubPosPair(reversehub,
			(reversehub.type() == nodeT::Hub) ? (hubPtr(reversehub).number() * 4 + revLocPos) : revLocPos));
	}
}
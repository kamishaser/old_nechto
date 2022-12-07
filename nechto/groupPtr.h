#pragma once
#include "nodePtr.h"
#include "hubPtr.h"
#include "hubPosPair.h"

namespace nechto
{
	class groupPtr : public existing<nodePtr>
	{
		friend class creator;
		friend class hubEraser;
		void setFirstGroupHub(hubPtr hub)
		{
			(reinterpret_cast<hubPosPair*>(&node()->data))->hub = hub;
		}
		void setSize(int32_t size)
		{
			(reinterpret_cast<hubPosPair*>(&node()->data))->setGlobalPos(size);
		}
	public:
		groupPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		hubPtr firstGroupHub() const
		{
			return existing<nodePtr>(getData<hubPosPair>().hub);
		}
		int32_t getSize() const
		{
			return getData<hubPosPair>().getGlobalPos();
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::Group;
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
	};
}
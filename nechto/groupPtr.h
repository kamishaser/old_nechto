#pragma once
#include "nodePtr.h"
#include "hubPtr.h"

namespace nechto
{
	class groupPtr : public existing<nodePtr>
	{
		friend class creator;
		friend class hubEraser;
		void setFirstGroupHub(hubPtr hub)
		{
			setData<nodePtr>(hub);
		}
	public:
		groupPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		hubPtr firstGroupHub()
		{
			return existing<nodePtr>(getData<nodePtr>());
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
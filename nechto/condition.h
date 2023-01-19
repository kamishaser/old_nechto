#pragma once
#include "nodePtr.h"

namespace nechto
{
	class conditionPtr : public existing<nodePtr>
	{
	public:
		conditionPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::Condition;
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
		operationData* opDataPtr()
		{
			return getDataPtr<operationData>();
		}
		
	};
}
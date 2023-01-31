#pragma once
#include "nodePtr.h"
#include "nodeOperationSet.h"
#include "crSet.h"
namespace nechto
{
	namespace pointerMovementOperationF
	{
	}
	namespace pointerMovementOperationO
	{
	}
	class pointerMovementOperationPtr : public existing<nodePtr>
	{
		friend class creator;
	public:
		pointerMovementOperationPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		pointerMovementOperationPtr(const nodePtr& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::PointerationOperation;
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

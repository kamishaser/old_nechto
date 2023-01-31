#pragma once
#include "nodePtr.h"
#include "nodeOperationSet.h"
#include "crSet.h"
namespace nechto
{
	namespace connectionOperationF
	{
	}
	namespace connectionOperationO
	{
	}
	class connectionOperationPtr : public existing<nodePtr>
	{
		friend class creator;
	public:
		connectionOperationPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		connectionOperationPtr(const nodePtr& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::ConnectionOperation;
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
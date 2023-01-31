#pragma once
#include "nodePtr.h"
#include "nodeOperationSet.h"
#include "crSet.h"
namespace nechto
{
	namespace vectorOperationF
	{
	}
	namespace vectorOperationO
	{
	}
	class vectorOperationPtr : public existing<nodePtr>
	{
		friend class creator;
	public:
		vectorOperationPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		vectorOperationPtr(const nodePtr& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::VectorOperation;
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
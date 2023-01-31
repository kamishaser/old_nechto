#pragma once
#include "nodePtr.h"
#include "nodeOperationSet.h"
#include "crSet.h"
namespace nechto
{
	namespace creationOperationF
	{
	}
	namespace creationOperationO
	{
	}
	class creationOperationPtr : public existing<nodePtr>
	{
		friend class creator;
	public:
		creationOperationPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		creationOperationPtr(const nodePtr& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::CreationOperation;
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
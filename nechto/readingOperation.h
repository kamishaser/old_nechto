#pragma once
#include "nodePtr.h"
#include "nodeOperationSet.h"
#include "crSet.h"
namespace nechto
{
	namespace readingOperationF
	{
	}
	namespace readingOperationO
	{
	}
	class readingOperationPtr : public existing<nodePtr>
	{
		friend class creator;
	public:
		readingOperationPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		readingOperationPtr(const nodePtr& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::ReadingOperation;
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
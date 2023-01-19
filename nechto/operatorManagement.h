#pragma once
#include "nodePtr.h"
namespace nechto
{
	class operatorManagementPtr : public existing<nodePtr>
	{
		friend class creator;
	public:
		operatorManagementPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		operatorManagementPtr(const nodePtr& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::OperatorManagement;
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
#pragma once
#include "nodePtr.h"
#include "connectionRule.h"

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
		const static connectionRule cRule;
		
	};
	const connectionRule conditionPtr::cRule
	{conRule::conType::I64Variable, conRule::useType::readOnly};
}
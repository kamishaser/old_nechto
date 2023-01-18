#pragma once
#include "entity.h"
#include "conRuleInterface.h"
#include "iteratorPtr.h"
#include "connectionIterator.h"

namespace nechto
{
	template<essT ECon>
	struct oneConnectionRule
	{
		elementaryRule elRule;
		iteratorRule iterRule;
		entityRule enRule;
	};
}

#pragma once
#include "node.h"
#include "visual.h"

namespace nechto::ide
{
	struct visualConnectionID
	{
		const nodePtr first;
		const nodePtr second;
		
		visualConnectionID(nodePtr f = nullNodePtr, nodePtr s = nullNodePtr)
			:first((f < s) ? f : s), second((f < s) ? s : f) {}

		bool operator< (const visualConnectionID& c2) const
		{
			if (first < c2.first)
				return true;
			else if (first == c2.first && second < c2.second)
				return true;
			return false;
		}
	};
	struct visualConnectionData
	{
		using oneConNumbers = std::pair<i64, i64>;
		std::vector<oneConNumbers> connections;
	};
}
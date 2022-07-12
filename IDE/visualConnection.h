#pragma once
#include "node.h"
#include "visual.h"

namespace nechto::ide
{
	struct visualConnectionID
	{
		const nodePtr first;
		const nodePtr second;
		visualConnectionID(nodePtr f, nodePtr s)
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
		using oneConNumbers = std::pair<int64_t, int64_t>;
		std::vector<oneConNumbers> connections;
	};
}
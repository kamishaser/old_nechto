#pragma once
#include "graph.h"
#include "periodLimiter.h"

namespace nechto::ide
{
	struct nodeBoard
	{
		std::vector<std::shared_ptr<graph::handler>> handlerSet;
		graph mainGraph;
		periodLimiter plim;

		nodeBoard()
			:plim(10ms, 12ms) {}

		nodeBoard(const graph& nGraph)
			:mainGraph(nGraph), plim(10ms, 12ms) {}

		void update()
		{
			if (plim.moreThanMin())
			{
				for (auto i = handlerSet.begin(); i != handlerSet.end(); ++i)
					if (*i)
						if((*i)->switchedOn)
							(*i)->update(plim.currentPeriod());
					else
						i = handlerSet.erase(i);
				plim.reset();
			}
		}
		void addHandler(std::shared_ptr<graph::handler> handler)
		{
			handler->nGraph = &mainGraph;
			handlerSet.push_back(handler);
		}

	};
}
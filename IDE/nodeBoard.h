#pragma once
#include "graph.h"
#include "periodLimiter.h"

namespace nechto::ide
{
	struct nodeBoard
	{
		std::vector<std::shared_ptr<graph::handler>> handlerSet;
		std::shared_ptr<graph> nGraph;
		periodLimiter plim;

		nodeBoard(std::shared_ptr<graph> nG)
			:nGraph(nG), plim(10ms, 12ms) {}

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
			for (auto i1 = nGraph->nodes.begin(); i1 != nGraph->nodes.end(); ++i1)
			{
				i1->second.position += i1->second.stepPosExchange;
				i1->second.stepPosExchange = glm::vec2(0, 0);
			}
		}
		void addHandler(std::shared_ptr<graph::handler> handler)
		{
			handler->nGraph = nGraph;
			handlerSet.push_back(handler);
		}

	};
}
#pragma once
#include "graph.h"
#include "periodLimiter.h"

namespace nechto::ide
{
	struct nodeBoard
	{
		std::vector<std::shared_ptr<graph::handler>> handlerSet;
		std::shared_ptr<graph> nGraph;
		nodeBoard(std::shared_ptr<graph> nG)
			:nGraph(nG){}

		void update()
		{

			for (auto i = handlerSet.begin(); i != handlerSet.end(); ++i)
				if (*i)
					if ((*i)->switchedOn)
						if ((*i)->plim.moreThanMin())
						{
							(*i)->update();
							(*i)->plim.reset();
						}
						else{}
					else{}
				else
				{
					std::cout << "erase handler" << std::endl;
					i = handlerSet.erase(i);
				}

		}
		void addHandler(std::shared_ptr<graph::handler> handler)
		{
			handler->nGraph = nGraph;
			handlerSet.push_back(handler);
		}

	};
}
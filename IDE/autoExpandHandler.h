#pragma once
#include "graph.h"
#include "periodLimiter.h"

namespace nechto::ide
{
	namespace handler
	{
		class autoExpandHandler : public  graph::handler
		{
		public:
			periodLimiter plim;

			autoExpandHandler()
				:plim(1000ms, 1020ms) {}
			virtual ~autoExpandHandler() override {}

			virtual void update(milliseconds timeInterval) override
			{
				if (plim.moreThanMin())
				{
					std::set<nodePtr> newNodes;
					std::set<visualConnectionID> newConnections;
					for (auto nodeI = nGraph->nodes.begin(); nodeI != nGraph->nodes.end(); ++nodeI)
					{
						connectionIterator i(nodeI->first, false);
						do
						{
							if (i.get().exist())
							{
								if (!nGraph->containsNode(i.get()))
								{
									newNodes.emplace(i.get());
									std::cout << "node added: " << to_string(i.get()) << std::endl;
								}
								newConnections.emplace(visualConnectionID(nodeI->first, i.get()));
								/*nGraph->determineConnectionNumbers(
									visualConnectionID(nodeI->first, i.get()));*/
							}
						} while (++i);
					}
					for (auto i : newNodes)
						nGraph->addNode(i);
					for (auto i : newConnections)
						nGraph->connect(i);

					plim.reset();
				}
			}
		};
	}
}

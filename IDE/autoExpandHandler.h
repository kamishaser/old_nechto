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
			autoExpandHandler()
				:graph::handler(1000ms, 1020ms) {}
			virtual ~autoExpandHandler() override {}

			virtual void update() override
			{
				std::set<nodePtr> newNodes;
				std::set<visualConnectionID> newConnections;
				for (auto nodeI = nGraph->nodes.begin(); nodeI != nGraph->nodes.end(); ++nodeI)
				{

					connectionIterator i(nodeI->first);
					do
					{
						if (!i.get().exist())
							continue;
						if (typeCompare(i.get(), node::Hub))
							continue;
						if (visualNode::getByPtr(i.get()) != nullptr)
							continue;
						
						
						newNodes.emplace(i.get());
						newConnections.emplace(visualConnectionID(nodeI->first, i.get()));
					} while (i.stepForward());
				}
				for (auto i : newNodes)
					nGraph->addNode(i);
				for (auto i : newConnections)
					nGraph->connect(i);
			}
		};
	}
}

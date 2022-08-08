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
						nodePtr v1 = i.get();
						if (!v1.exist())
							continue;
						if (typeCompare(v1, node::ExternalConnection))
						{
							auto ptr = v1->getData<externalConnection*>();
							if (ptr != nullptr)
								if (ptr->getTypeName() == visualNode::vnTypeName)
									continue;
						}
						if (nGraph->containsNode(i.get()))
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

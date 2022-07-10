#pragma once
#include "graph.h"

namespace nechto::ide
{
	namespace handler
	{
		class autoExpandHandler : public  graph::handler
		{
		public:
			virtual ~autoExpandHandler() override {}

			virtual void update(milliseconds timeInterval) override
			{
				for (auto nodeI = nGraph->nodes.begin(); nodeI != nGraph->nodes.end(); ++nodeI)
				{
					connectionIterator i(nodeI->first, false);
					do
					{
						if (!nGraph->containsNode(i.get()))
						{
							nGraph->addNode(i.get());
							std::cout << "node added: " << to_string(i.get()) << std::endl;
						}
						nGraph->determineConnectionNumbers(
							visualConnectionID(nodeI->first, i.get()));
					} while (++i);
				}
			}
		};
	}
}

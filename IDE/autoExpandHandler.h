#pragma once
#include "periodLimiter.h"
#include "nodeBoard.h"

namespace nechto::ide
{
	/*class autoExpandHandler
	{
	public:
		nodePtr nBoardNode;
		autoExpandHandler(nodePtr nbn)
			:nBoardNode(nbn) {}
		void update()
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
					if (visualNode::getByNode(i.get()))
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
	};*/
}

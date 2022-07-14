#pragma once
#include "ideWindow.h"
#include "graph.h"

namespace nechto::ide
{
	class userH : public graph::handler
	{
		std::shared_ptr<ideWindow> window;
		std::set<nodePtr> highlightedNodes;
		std::set<visualConnectionID> highlightedConnections;

		visualConnectionID mouseOveredElement;

	};
}
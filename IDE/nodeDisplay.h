#pragma once
#include "graph.h"
#include "periodLimiter.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML GML compatibility.h"
#include "visualNode.h"
namespace nechto::ide
{
	
	//class nodeDisplay
	//{
	//	graph& nGraph;
	//	sf::RenderWindow& window;
	//public:
	//	nodeDisplay(graph& nb, sf::RenderWindow& rv)
	//		:nGraph(nb), window(rv) {}

	//	void update()
	//	{
	//		std::set<std::pair<nodePtr, nodePtr>> drawedConnections;

	//		for (auto i : nGraph.vNode)
	//		{
	//			/*for (auto i2 : i->connections)
	//			{
	//				nodePtr v1 = *i;
	//				nodePtr v2 = *i2.second.lock();
	//				if (v1 > v2)
	//					std::swap(v1, v2);
	//				std::pair<nodePtr, nodePtr> con(v1, v2);
	//				if (!drawedConnections.contains(con))
	//				{
	//					drawedConnections.emplace(con);
	//					sf::ConvexShape connection = line(i->position, i2.second.lock()->position, 6);
	//					connection.setFillColor(sf::Color::Blue);
	//					window.draw(connection);
	//				}
	//			}*/
	//		}
	//		for (auto i : nGraph.vNode)
	//		{
	//			visualNode vNode(i);
	//			

	//			
	//		}
	//	}
	//};
}
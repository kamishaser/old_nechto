#pragma once
#include "nodeBoard.h"
#include "periodLimiter.h"
#include "line.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML GML compatibility.h"
#include "visualNode.h"
namespace nechto::ide
{
	
	class nodeDisplay
	{
		nodeBoard& nBoard;
		sf::RenderWindow& window;
	public:
		nodeDisplay(nodeBoard& nb, sf::RenderWindow& rv)
			:nBoard(nb), window(rv) {}

		void update()
		{
			std::set<std::pair<nodePtr, nodePtr>> drawedConnections;

			for (auto i : nBoard.vNode)
			{
				for (auto i2 : i->connections)
				{
					nodePtr v1 = *i;
					nodePtr v2 = *i2.second.lock();
					if (v1 > v2)
						std::swap(v1, v2);
					std::pair<nodePtr, nodePtr> con(v1, v2);
					if (!drawedConnections.contains(con))
					{
						drawedConnections.emplace(con);
						sf::ConvexShape connection = line(i->position, i2.second.lock()->position, 6);
						connection.setFillColor(sf::Color::Red);
						window.draw(connection);
					}
				}
			}
			for (auto i : nBoard.vNode)
			{
				visualNode vNode(i);
				/*switch (i->operator nechto::node::ptr()->getType())
				{
				case node::Error:
					shape.setFillColor(sf::Color::Red);
					break;
				case node::Hub:
					shape.setFillColor(sf::Color(50, 50, 50));
					break;
				case node::Variable:
					shape.setFillColor(sf::Color(200, 200, 200));
					break;
				case node::TypeCastOperator:
					shape.setFillColor(sf::Color(0, 200, 200));
					break;;
				case node::MathOperator:
					shape.setFillColor(sf::Color::Blue);
					break;
				case node::Tag:
					shape.setFillColor(sf::Color::Green);
					break;
				case node::ConditionalBranching:
					shape.setFillColor(sf::Color::Yellow);
					break;
				case node::ExteralFunction:
					shape.setFillColor(sf::Color::Magenta);
					break;
				case node::Pointer:
					shape.setFillColor(sf::Color::Magenta);
					break;
				}*/

				vNode.draw(window);
			}
		}
	};
}
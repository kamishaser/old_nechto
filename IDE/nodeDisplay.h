#pragma once
#include "nodeBoard.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "GLM/trigonometric.hpp"
#include "GLM/gtx/rotate_vector.hpp"

namespace nechto::ide
{
	sf::Vector2f SfGlmvec2(glm::vec2 v)
	{
		return sf::Vector2f(v.x, v.y);
	}
	class nodeDisplay
	{
		nodeBoard& nBoard;
		sf::RenderWindow& window;

		static sf::ConvexShape getLine(glm::vec2 first, glm::vec2 second, float size)
		{
			glm::vec2 temp = second - first;
			glm::vec3 normal(temp.x, temp.y, 0);
			normal = glm::rotateZ(normal, 3.14f / 2);
			temp = glm::vec2(normal.x, normal.y);
			temp = glm::normalize(temp) * (size / 2);
			sf::ConvexShape line(4);
			line.setPosition(0, 0);
			line.setPoint(0, SfGlmvec2(first + temp));
			line.setPoint(1, SfGlmvec2(first - temp));
			line.setPoint(2, SfGlmvec2(second - temp));
			line.setPoint(3, SfGlmvec2(second + temp));
			return line;
		}
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
						sf::ConvexShape connection = getLine(i->position, i2.second.lock()->position, 6);
						connection.setFillColor(sf::Color::Red);
						window.draw(connection);
					}
				}
			}
			for (auto i : nBoard.vNode)
			{
				sf::CircleShape shape(i->size);
				shape.setPosition(SfGlmvec2(i->position - glm::vec2(i->size, i->size)));

				switch (i->operator nechto::node::ptr()->getType())
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
				}

				window.draw(shape);
			}
		}
	};
}
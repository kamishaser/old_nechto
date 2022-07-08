#pragma once
#include "SFML/Graphics.hpp"
#include "locatedNode.h"
#include "GLM/glm.hpp"
#include "SFML GML compatibility.h"

namespace nechto::ide
{
	class visualNode
	{

		
		sf::CircleShape shape;
		sf::Text  text;
		sharedLocatedNode vNode;
		
	public:
		static sf::Font Font;
		visualNode(sharedLocatedNode n)
			:vNode(n), 
			shape(0.5f)
		{
			text.setFont(Font);
			text.setCharacterSize(20);
			shape.setFillColor(sf::Color::Green);
		}
		void draw(sf::RenderWindow& window)
		{
			compute();
			window.draw(shape);
			window.draw(text);
		}
		sf::Vector2f getCenterPosition()
		{
			return GLM_SFML(vNode->position);
		}
		sf::Vector2f getTopLeftPosition()
		{
			return GLM_SFML(vNode->position - (vNode->size / 2));
		}
		float getSize()
		{
			return vNode->size;
		}
	private:
		void compute()
		{
			computeText();
			shape.setPosition(getTopLeftPosition());
			shape.setRadius(vNode->size/2);
		}
		void computeText()
		{
			text.setOutlineColor(sf::Color::Black);
			text.setOutlineThickness(1);
			text.setString("laja");

			sf::FloatRect bounds = text.getLocalBounds();
			glm::vec2 diagonal(bounds.width, bounds.height);
			vNode->size = ((glm::length(diagonal)));
			std::cout << vNode->size << std::endl;
			text.setPosition(GLM_SFML(vNode->position - (glm::normalize(diagonal)*vNode->size*0.5f)));
		}

	};
	sf::Font visualNode::Font;
}

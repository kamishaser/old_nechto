#pragma once
#include "SFML/Window.hpp"
#include "SFML GML compatibility.h"
#include "GLM/gtx/rotate_vector.hpp"
#include "visualNode.h"
#include "visualConnection.h"

namespace nechto::ide
{
	class display
	{
	public:
		virtual bool exist() { return false; };
		virtual bool active() { return false; };
		virtual ~display() {}

		virtual void draw(visualNode&) {};
		virtual void draw(visualNode& first, visualNode& second, visualConnectionData& vCon){};
	};

	class sfmlDisplay : public display
	{
	public:
		sf::Font vnFont;
		sf::RenderWindow& window;

		sfmlDisplay(sf::RenderWindow& w)
			:window(w) 
		{
			assert(vnFont.loadFromFile("Fonts/arial.ttf"));
		}

		virtual ~sfmlDisplay() {}
		virtual bool exist() { return window.isOpen(); }
		virtual bool active() { return true; };


		virtual void draw(visualNode& vNode) override;
		virtual void draw(visualNode& first, visualNode& second, visualConnectionData& vCon) override;
	};
	void sfmlDisplay::draw(visualNode& vNode)
	{
		//////////////////////////////////////////////////////////////////
		sf::Text text;
		text.setFont(vnFont);
		text.setString(vNode.nodeText);
		text.setCharacterSize(18);
		text.setStyle(1);
		//////////////////////////////////////////////////////////////////
		sf::FloatRect bounds = text.getLocalBounds();
		glm::vec2 size(bounds.width, bounds.height);
		text.setPosition(GLM_SFML(vNode.position - (size)+glm::vec2(500,500)));
		vNode.size = size * 1.1f;
		//////////////////////////////////////////////////////////////////
		sf::ConvexShape nShape(vNode.nShape.size());
		nShape.setPosition(sf::Vector2f(500, 500));
		if (!vNode.nShape.get().empty())
			for (int i = 0; i < vNode.nShape.size(); ++i)
			{
				float x = vNode.nShape[i].x * vNode.size.x;
				float y = vNode.nShape[i].y * vNode.size.y;
				glm::vec2 pointPosition(glm::vec2(x, y) + vNode.position);
				nShape.setPoint(i, GLM_SFML(pointPosition));
			}
		else
		{
			std::cout << "laja";
			nShape.setPointCount(4);
			glm::vec2 pointPosition;

			pointPosition = vNode.position;
			pointPosition += glm::vec2(-vNode.size.x, -vNode.size.y) / 2.0f;
			nShape.setPoint(0, GLM_SFML(pointPosition));

			pointPosition = vNode.position;
			pointPosition += glm::vec2(vNode.size.x, -vNode.size.y) / 2.0f;
			nShape.setPoint(1, GLM_SFML(pointPosition));

			pointPosition = vNode.position;
			pointPosition += glm::vec2(vNode.size.x, vNode.size.y) / 2.0f;
			nShape.setPoint(2, GLM_SFML(pointPosition));

			pointPosition = vNode.position;
			pointPosition += glm::vec2(-vNode.size.x, vNode.size.y) / 2.0f;
			nShape.setPoint(3, GLM_SFML(pointPosition));
		}
		//////////////////////////////////////////////////////////////////
		sf::Color shc = nShape.getFillColor();
		int r = (shc.r > 64) ? 32 : 255;
		int g = (shc.g > 128) ? 4 : 255;
		int b = (shc.b > 64) ? 32 : 223;
		text.setFillColor(sf::Color(r, g, b));
		//////////////////////////////////////////////////////////////////
		std::cout << nShape.getPointCount();
		std::cout << nShape.getPoint(0).x << ' ' << nShape.getPoint(0).y << std::endl;
		window.draw(nShape);
		window.draw(text);
	}
	void sfmlDisplay::draw
	(visualNode& first, visualNode& second, visualConnectionData& vCon)
	{
		////////////////////////////////////////////////////////////////////////
		float thickness = 6;
		glm::vec2 fpos = first.position;
		glm::vec2 spos = second.position;
		////////////////////////////////////////////////////////////////////////
		sf::ConvexShape line;
		glm::vec2 temp = spos - fpos;
		glm::vec3 normal(temp.x, temp.y, 0);
		normal = glm::rotateZ(normal, 3.14f / 2);
		temp = glm::vec2(normal.x, normal.y);
		temp = glm::normalize(temp) * (thickness / 2);
		////////////////////////////////////////////////////////////////////////
		line.setPosition(500, 500);
		line.setPoint(0, GLM_SFML(fpos + temp));
		line.setPoint(1, GLM_SFML(fpos - temp));
		line.setPoint(2, GLM_SFML(spos - temp));
		line.setPoint(3, GLM_SFML(spos + temp));
		////////////////////////////////////////////////////////////////////////
		line.setFillColor(sf::Color::Blue);
		////////////////////////////////////////////////////////////////////////
		window.draw(line);
	}
}

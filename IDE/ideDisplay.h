#pragma once
#include "SFML/Window.hpp"
#include "GLM/gtx/rotate_vector.hpp"
#include "nodeBoard.h"

namespace nechto::ide
{
	class ideDisplay
	{
	public:
		sf::Font vnFont;
		sf::RenderWindow window;
		periodLimiter plim;

		struct settings
		{
			float baseLineThickness = 6;
			int characterSize = 18;
			
		};
		settings dSettings;

		ideDisplay()
			:window(sf::VideoMode(1000, 1000), "nechtoIDE"),
			plim(20ms, 100ms)
		{
			assert(vnFont.loadFromFile("Fonts/arial.ttf"));
		}

		glm::vec2 getSize()
		{
			return glm::vec2(window.getSize().x, window.getSize().y);
		}
		glm::vec2 getCursorPosition()
		{
			return glm::vec2(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		}
		virtual bool update(std::shared_ptr<graph> nGraph)
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
					return false;
				}
			}
			if (plim.moreThanMin())
			{
				window.clear();
				for (auto i1 = nGraph->connections.begin(); i1 != nGraph->connections.end(); ++i1)
					draw(
						nGraph->findNode(i1->first.first),
						nGraph->findNode(i1->first.second), i1->second);
				for (auto i1 = nGraph->nodes.begin(); i1 != nGraph->nodes.end(); ++i1)
					draw(i1->second);
				window.display();
				plim.reset();
			}
		}

		void draw(visualNode& vNode)
		{
			//////////////////////////////////////////////////////////////////
			sf::Text text;
			
			text.setFont(vnFont);
			text.setString(vNode.nodeText);
			text.setCharacterSize(dSettings.characterSize);
			text.setStyle(1);
			//////////////////////////////////////////////////////////////////
			sf::FloatRect bounds = text.getLocalBounds();
			glm::vec2 size(bounds.width, bounds.height);
			vNode.size = size + glm::vec2(dSettings.characterSize, dSettings.characterSize);
			text.setPosition(sf::Vector2f(
				vNode.position.x - bounds.width / 2.0f,
				vNode.position.y - bounds.height / 2.0f - dSettings.characterSize/4));

			//////////////////////////////////////////////////////////////////


			if (vNode.nShape.empty())
				vNode.nShape = std::vector<glm::vec2>{
					glm::vec2(-0.5,-0.5),
					glm::vec2(0.5 ,-0.5),
					glm::vec2(0.5 , 0.5),
					glm::vec2(-0.5, 0.5) };
			sf::ConvexShape nShape(vNode.nShape.size());
			nShape.setPosition(0, 0);
			for (int i = 0; i < vNode.nShape.size(); ++i)
			{
				float x = vNode.nShape[i].x * vNode.size.x;
				float y = vNode.nShape[i].y * vNode.size.y;
				glm::vec2 pointPosition(glm::vec2(x, y) + vNode.position);
				nShape.setPoint(i, GLM_SFML(pointPosition));
			}
			//////////////////////////////////////////////////////////////////
			sf::Color shc = nShape.getFillColor();
			int r = (shc.r > 64) ? 32 : 255;
			int g = (shc.g > 128) ? 4 : 255;
			int b = (shc.b > 64) ? 32 : 223;
			text.setFillColor(sf::Color(r, g, b));
			//////////////////////////////////////////////////////////////////
			if (vNode.lightColor != sf::Color(0, 0, 0))
			{
				nShape.setOutlineColor(vNode.lightColor);
				nShape.setOutlineThickness(dSettings.baseLineThickness);
			}
			//////////////////////////////////////////////////////////////////

			window.draw(nShape);
			window.draw(text);
		}
		/////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////
		void draw(visualNode& first, visualNode& second, visualConnectionData& vCon)
		{
			////////////////////////////////////////////////////////////////////////
			glm::vec2 fpos = first.position;
			glm::vec2 spos = second.position;
			////////////////////////////////////////////////////////////////////////
			sf::ConvexShape line(4);
			glm::vec2 temp = spos - fpos;
			glm::vec3 normal(temp.x, temp.y, 0);
			normal = glm::rotateZ(normal, 3.14f / 2);
			temp = glm::vec2(normal.x, normal.y);
			temp = glm::normalize(temp) * (dSettings.baseLineThickness / 2);
			////////////////////////////////////////////////////////////////////////
			line.setPoint(0, GLM_SFML(fpos + temp));
			line.setPoint(1, GLM_SFML(fpos - temp));
			line.setPoint(2, GLM_SFML(spos - temp));
			line.setPoint(3, GLM_SFML(spos + temp));
			////////////////////////////////////////////////////////////////////////
			line.setFillColor(sf::Color::Blue);
			////////////////////////////////////////////////////////////////////////
			window.draw(line);
		}
	};
}

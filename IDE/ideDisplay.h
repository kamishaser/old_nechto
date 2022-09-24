#pragma once
#include "SFML/Window.hpp"
#include "GLM/gtx/rotate_vector.hpp"
#include "nodeBoard.h"
#include "periodLimiter.h"

namespace nechto::ide
{
	class ideDisplay : public externalObject
	{
	public:
		sf::Font vnFont;
		sf::RenderWindow window;
		periodLimiter plim;
		nodeBoard* nBoard;

		struct settings
		{
			float baseLineThickness = 6;
			int characterSize = 18;
			
		};
		settings dSettings;

		ideDisplay(nodeBoard* nbn)
			:externalObject(newNode(node::ExternalObject, 1)),
			nBoard(nbn), window(sf::VideoMode(1000, 1000), "nechtoIDE"),
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
		virtual bool update()
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
				plim.reset();
				assert(nBoard);
				window.clear();
				groupIterator i1(nBoard->vConnectionGroup());
				do
				{
					visualConnection* vscon = visualConnection::getByNode(i1.get());
					if (vscon)
						draw(vscon);
				} while (i1.stepForward());
				groupIterator i2(nBoard->vNodeGroup());
				do
				{
					visualNode* vNode = visualNode::getByNode(i2.get());
					if (vNode)
						draw(vNode);
				} while (i2.stepForward());
				window.display();
			}
			return true;
		}

		void draw(visualNode* vNode)
		{
			//////////////////////////////////////////////////////////////////
			sf::Text text;
			
			text.setFont(vnFont);
			text.setCharacterSize(dSettings.characterSize);
			text.setStyle(1);
			text.setString(vNode->nodeText);
			text.setPosition(0, 0);
			//////////////////////////////////////////////////////////////////
			sf::FloatRect bounds = text.getGlobalBounds();
			glm::vec2 size(bounds.width, bounds.height);
			size.x += dSettings.characterSize / 2;
			size.y += dSettings.characterSize / 2;
			//size += size + glm::vec2(dSettings.characterSize/2, dSettings.characterSize/2);
			if (vNode->frame.size.x < size.x)
				vNode->frame.size.x = size.x;
			if (vNode->frame.size.y < size.y)
				vNode->frame.size.y = size.y;
			text.setPosition(sf::Vector2f(
				vNode->frame.position.x + dSettings.characterSize / 4,
				vNode->frame.position.y + dSettings.characterSize / 6));
			//////////////////////////////////////////////////////////////////

			if (vNode->nShape.empty())
				vNode->nShape = vnShape::rectangle();
			sf::ConvexShape nShape(vNode->nShape.size());
			for (int i = 0; i < vNode->nShape.size(); ++i)
			{
				float x = vNode->nShape[i].x * vNode->frame.size.x;
				float y = vNode->nShape[i].y * vNode->frame.size.y;
				glm::vec2 pointPosition(glm::vec2(x, y) + vNode->frame.center());
				nShape.setPoint(i, GLM_SFML(pointPosition));
			}
			//////////////////////////////////////////////////////////////////
			sf::Color shc = nShape.getFillColor();
			int r = (shc.r > 64) ? 32 : 255;
			int g = (shc.g > 128) ? 4 : 255;
			int b = (shc.b > 64) ? 32 : 223;
			text.setFillColor(sf::Color(r, g, b));
			//////////////////////////////////////////////////////////////////
			if (vNode->lightColor != sf::Color(0, 0, 0))
			{
				nShape.setOutlineColor(vNode->lightColor);
				nShape.setOutlineThickness(dSettings.baseLineThickness);
			}
			//////////////////////////////////////////////////////////////////

			window.draw(nShape);
			window.draw(text);
		}
		/////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////
		void draw(visualConnection* vCon)
		{
			visualNode* first = visualNode::getByNode(vCon->get()->connection[0]);
			visualNode* second = visualNode::getByNode(vCon->get()->connection[1]);
			assert(first && second);
			////////////////////////////////////////////////////////////////////////
			glm::vec2 fpos = first->frame.position;
			glm::vec2 spos = second->frame.position;
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
		static ideDisplay* getByNode(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalObject)
				return nullptr;
			return dynamic_cast<ideDisplay*>(v1->getData<externalObject*>());
		}
		const static std::wstring typeName;
		virtual const std::wstring& getTypeName() const override
		{
			return typeName;
		}
	};
	const std::wstring ideDisplay::typeName = L"nechtoIde.ideDisplay";
}

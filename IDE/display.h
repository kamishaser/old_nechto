#pragma once

#include "textInputBox.h"
#include "SFML/Window.hpp"
#include "GLM/gtx/rotate_vector.hpp"
#include "periodLimiter.h"
#include "nodeBoard.h"
#include "visualGroup.h"
#include <memory>
#include <filesystem>

namespace nechto::ide
{
	//временный класс. Будет удалён или изменён после появления нормального интерфейса
	struct display
	{
		sf::RenderWindow window;
		sf::Font vnFont;
		struct settings
		{
			float baseLineThickness = 6;
			int characterSize = 18;

		};
		settings dSettings;




		display(const std::filesystem::path& path)
			:window(sf::VideoMode(1000, 1000), "nechtoIDE")
		{
			//загрузка базового шрифта
			assert(vnFont.loadFromFile((path / L"Fonts/arial.ttf").generic_string()));
			//добавление групп на доски
		}
		rect windowRect() const
		{
			return rect(glm::vec2(0, 0), SFML_GLM(window.getSize()));
		}
		virtual bool update(nodeBoard& workBoard, nodeBoard& interfaceBoard,
			textInputBox& textBox)
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
					return false;
				}
				else if (event.type == sf::Event::Resized)
				{
					rect winRect = windowRect();
					window.setView(sf::View(GLM_SFML(winRect.center()),
						GLM_SFML(winRect.size)));
				}
				else if (event.type == sf::Event::TextEntered)
					textBox.update(event.text.unicode);
				/*else if (event.type == sf::Event::MouseWheelMoved)
				{
					static int mScrollData = event.mouseWheelScroll.x;
					sf::View View = window.getView();
					std::cout << event.mouseWheelScroll.x << std::endl;
					View.zoom((event.mouseWheel.delta > 0) ? 0.9f : 1.1f);
					mScrollData = event.mouseWheelScroll.x;
					window.setView(View);
				}*/
			}


			window.clear(col::vGroupBackground);
			drawBoard(&workBoard);
			drawBoard(&interfaceBoard);
			window.display();

			return true;
		}
	private:
		void drawBoard(nodeBoard* nBoard)
		{

			assert(nBoard);

			groupPointer i1(nBoard->vConnectionGroup());
			do
			{
				if(entityPtr<visualConnection>::match(i1.get()))
					draw(entityPtr<visualConnection>(i1.get()));
			} while (i1.stepForward());
			groupPointer i2(nBoard->vNodeGroup());
			do
			{
				if (entityPtr<visualNode>::match(i2.get()))
					draw(entityPtr<visualNode>(i2.get()).get());
			} while (i2.stepForward());

		}
	public:

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
			nShape.setFillColor(vNode->shapeColor);
			text.setFillColor(col::sel[2]);
			for (int i = 0; i < vNode->nShape.size(); ++i)
			{
				float x = vNode->nShape[i].x * vNode->frame.size.x;
				float y = vNode->nShape[i].y * vNode->frame.size.y;
				glm::vec2 pointPosition(glm::vec2(x, y) + vNode->frame.center());
				nShape.setPoint(i, GLM_SFML(pointPosition));
			}
			//////////////////////////////////////////////////////////////////
			//text.setFillColor(sf::Color);
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
		void draw(entityPtr<visualConnection> vCon)
		{
			visualNode* first = getEntity<visualNode>(vCon.connection(0));
			visualNode* second = getEntity<visualNode>(vCon.connection(1));

			assert((first) && (second));
			////////////////////////////////////////////////////////////////////////
			const glm::vec2 fpos = first->frame.center();
			const glm::vec2 spos = second->frame.center();
			////////////////////////////////////////////////////////////////////////
			sf::ConvexShape line(4);
			glm::vec2 temp = spos - fpos;
			glm::vec2 quarter = temp / 4.2f;
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
			line.setFillColor(col::strong);
			////////////////////////////////////////////////////////////////////////
			window.draw(line);
			////////////////////////////////////////////////////////////////////////
			
			

			if (!vCon->fText.empty())
			{
				visualNode vNode1(nullptr, nullptr);
				vNode1.frame.position = fpos + quarter
					- glm::vec2(dSettings.characterSize, dSettings.characterSize);
				vNode1.nodeText = vCon->fText;
				vNode1.shapeColor = col::strong;
				draw(&vNode1);
			}
			if (!vCon->sText.empty())
			{
				visualNode vNode2(nullptr, nullptr);
				vNode2.frame.position = spos - quarter;
				vNode2.nodeText = vCon->sText;
				vNode2.shapeColor = col::strong;
				draw(&vNode2);
			}
		}
	};
}
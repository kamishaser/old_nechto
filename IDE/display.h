#pragma once
#include "visualGroup.h"
#include "consistentGroup.h"
#include "nodeBoard.h"
#include "textInputBox.h"
#include "textOut.h"

#include "fileSerializer.h"
#include "SFML/Window.hpp"
#include "GLM/gtx/rotate_vector.hpp"
#include "periodLimiter.h"
#include <memory>

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

		nodeBoard workBoard;
		consistentGroup leftChain{ newExObjNode(0) };
		consistentGroup rightChain{ newExObjNode(0) };
		visualGroup center = { newExObjNode(0) };

		nodeBoard interfaceBoard;
		consistentGroup topGroup{ newExObjNode(0) };
		consistentGroup bottomGroup{ newExObjNode(0) };

		visualNode cursoredParametrs{ newExObjNode(0) };
		visualNode cursoredConnections{ newExObjNode(0) };
		visualNode textBoxNode{ newExObjNode(0) };
		visualNode consoleOut{ newExObjNode(0) };
		textInputBox  textBox;
		

		display()
			:window(sf::VideoMode(1000, 1000), "nechtoIDE"),
			textBox(textBoxNode.get())
		{
			//загрузка базового шрифта
			assert(vnFont.loadFromFile("Fonts/arial.ttf"));
			//добавление групп на доски
			workBoard.addGroup(&leftChain);
			workBoard.addGroup(&rightChain);
			workBoard.addGroup(&center);
			
			interfaceBoard.addGroup(&topGroup);
			interfaceBoard.addGroup(&bottomGroup);
			//установка режима позицианирования
			rightChain.mode.rightAlignment = true;
			topGroup.mode.horisontal = true;
			bottomGroup.mode.horisontal = true;
			bottomGroup.mode.rightAlignment = true;
			//bottomGroup.mode.rightAlignment = true;
			//добавление элементов интерфейса
			interfaceBoard.addNode(&cursoredParametrs);
			interfaceBoard.addNode(&cursoredConnections);
			interfaceBoard.addNode(&textBoxNode);
			interfaceBoard.addNode(&consoleOut);

			bottomGroup.addNode(&cursoredParametrs);
			bottomGroup.addNode(&cursoredConnections);
			bottomGroup.addNode(&textBoxNode);
			bottomGroup.addNode(&consoleOut);
			consoleText = &consoleOut.nodeText;

			cursoredParametrs.nodeText = L"наведи на ноду для получения данных";

			setChainsPosition();
		}
		void setChainsPosition()
		{
			leftChain.setPositionByStartPoint(glm::vec2(10, 10));
			rightChain.setPositionByStartPoint(glm::vec2(window.getSize().x - 10, 10));
			center.frame.setPositionByCenter(SFML_GLM(window.getSize()) / 2.f);
			topGroup.setPositionByStartPoint(glm::vec2(10, 10));
			bottomGroup.setPositionByStartPoint(glm::vec2(10, window.getSize().y - 10));
			
		}
		rect windowRect() const
		{
			return rect(glm::vec2(0, 0), SFML_GLM(window.getSize()));
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
				else if (event.type == sf::Event::Resized)
				{
					rect winRect = windowRect();
					window.setView(sf::View(GLM_SFML(winRect.center()), 
						GLM_SFML(winRect.size)));
					setChainsPosition();
				} else if (event.type == sf::Event::TextEntered)
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
			
			setChainsPosition();
			rightChain.update();
			leftChain.update();
			topGroup.update();
			bottomGroup.update();

			groupIterator gi(interfaceBoard.vNodeGroup());
			do
			{
				auto vNode = visualNode::getByNode(gi.get());
				if (vNode)
					vNode->frame.size = glm::vec2(0, 0);
			} while (gi.stepForward());
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
			nShape.setFillColor(col::vNodeG1);
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
		void draw(visualConnection* vCon)
		{
			visualNode* first = visualNode::getByNode(vCon->get()->connection[0]);
			visualNode* second = visualNode::getByNode(vCon->get()->connection[1]);
			assert(first && second);
			////////////////////////////////////////////////////////////////////////
			glm::vec2 fpos = first->frame.center();
			glm::vec2 spos = second->frame.center();
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
			line.setFillColor(col::strong);
			////////////////////////////////////////////////////////////////////////
			window.draw(line);
		}

		bool load(std::filesystem::path path)
		{
			fileDeserializer ds;
			ds.open(path);
			if (!ds.isOpen())
				return false;
			nodePtr temp = ds.deserialize();
			while (temp.exist())
			{
				auto vNode = new visualNode(newExObjNode(), temp);
				workBoard.addNode(vNode);
				temp = ds.deserialize();
			}
			ds.close();
		}
		bool save(std::filesystem::path path)
		{
			fileSerializer fs;
			fs.open(path);
			if (!fs.isOpen())
				return false;
			groupIterator gi(workBoard.vNodeGroup());
			do
			{
				auto vNode = visualNode::getByNode(gi.get());
				if (vNode && vNode->getConnection(0).exist())
					fs.serialize(vNode->getConnection(0));
			} while (gi.stepForward());
			fs.close();
		}
	};
}
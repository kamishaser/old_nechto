#pragma once
#include "drawableBlock.h"
#include "drawableLine.h"
#include "entity.h"
#include "structure.h"
#include "ideStructurePack.h"
#include <filesystem>
#include <memory>

namespace nechto::ide
{
	
	class drawer
	{
		const path vPath = "0"_np;
		const std::filesystem::path p;
	public:
		sf::Font font;
		drawer()
		{
			assert(font.loadFromFile((p / L"Fonts/arial.ttf").generic_string()));
		}

		bool update(sf::RenderWindow* window, nodePtr wNode) const
		{
			
			nodePtr workBoard;
			nodePtr interfaceBoard;

			if (!(
				sPack::window::workBoard / wNode >> workBoard &&
				sPack::window::interfaceBoard / wNode >> interfaceBoard))
				return false;

			window->clear(col::background);
			drawNodeBoard(window, workBoard);
			drawNodeBoard(window, interfaceBoard);
			window->display();
		}
	private:
		bool drawNodeBoard(sf::RenderWindow* window, nodePtr nodeBoard) const
		{
			std::unique_ptr<sf::Drawable> drawableElement;
			nodePtr vNodeGroup, vConnectionGroup, vGroupGroup;
			if (!(
				sPack::nBoard::vNodeGroup / nodeBoard >> vNodeGroup &&
				sPack::nBoard::vConnectionGroup / nodeBoard >> vConnectionGroup &&
				sPack::nBoard::vGroupGroup / nodeBoard >> vGroupGroup
				))return false;
			if (!(groupPtr::match(vConnectionGroup) &&
				groupPtr::match(vNodeGroup) && groupPtr::match(vGroupGroup)))
				return false;//защита от некоректных типов нод
			groupPointer iter(vGroupGroup);
			do
			{
				if (!iter.get().exist())
					continue;
				if (readVGroup(nodeBoard, vPath / iter.get(), drawableElement))
					window->draw(*drawableElement);
			} while (iter.stepForward());
			iter = groupPointer(vConnectionGroup);
			do
			{
				if (!iter.get().exist())
					continue;
				if (readVConnection(nodeBoard, vPath / iter.get(), drawableElement))
					window->draw(*drawableElement);
			} while (iter.stepForward());
			iter = groupPointer(vNodeGroup);
			do
			{
				if (!iter.get().exist())
					continue;
				if (readVNode(nodeBoard, vPath / iter.get(), drawableElement))
					window->draw(*drawableElement);
			} while (iter.stepForward());
		}
		bool readVGroup(nodePtr nodeBoard, nodePtr vGroup, 
			std::unique_ptr<sf::Drawable>& drawableElement) const
		{
			return false;
		}
		bool readVNode(nodePtr nodeBoard, nodePtr vNode, 
			std::unique_ptr<sf::Drawable>& drawableElement) const
		{//считывает информацию о ноде и заполняет drawableTextBlock
			
			drawableElement = std::make_unique<drawableTextBlock>(
				sPack::vNode::drawableBlock / vNode, font);
			return true;
		}
		bool readVConnection(nodePtr nodeBoard, nodePtr vConnection, 
			std::unique_ptr<sf::Drawable>& drawableElement) const
		{//считывает информацию о соединении и заполняет drawableTextLine
			f64 fX, fY, sX, sY, lineThinkness;
			nodePtr v0 = sPack::vConnection::verticle0 / vConnection;
			nodePtr v1 = sPack::vConnection::verticle1 / vConnection;
			if (!(
				sPack::vec2::x / v0 >> fX &&
				sPack::vec2::y / v0 >> fY &&
				sPack::vec2::x / v1 >> sX &&
				sPack::vec2::y / v1 >> sY))
				return false;
			sPack::vConnection::thinkness / vConnection >> lineThinkness;
			i64 lineColor = 1;
			sPack::vConnection::color / vConnection >> lineColor;

			drawableElement = std::make_unique<drawableLine>(
				glm::vec2(fX, fY), glm::vec2(sX, sY),
				lineColor, lineThinkness);
			return true;
		}
		
	};
}
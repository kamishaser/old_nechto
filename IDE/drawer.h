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
			f64 lineThinkness = 5.;
			nodePtr vNode0 = sPack::vConnection::vNode0 / vConnection;
			nodePtr vNode1 = sPack::vConnection::vNode1 / vConnection;
			nodePtr rectNode0 = sPack::vNode::frame / vNode0;
			nodePtr rectNode1 = sPack::vNode::frame / vNode1;
			rect rect0, rect1;
			if (!(rectNode0 >> rect0 && rectNode1 >> rect1))
				return false;
			sPack::vConnection::thinkness / vConnection >> lineThinkness;
			i64 lineColor = 1;
			sPack::vConnection::color / vConnection >> lineColor;

			drawableElement = std::make_unique<drawableLine>(
				rect0.center(), rect1.center(),	lineColor, lineThinkness);
			return true;
		}
		
	};
}
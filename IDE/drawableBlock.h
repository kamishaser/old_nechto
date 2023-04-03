#pragma once
#include "SFML/Graphics.hpp"
#include "GLM/gtx/rotate_vector.hpp"
#include "GLM/glm.hpp"
#include "rect.h"

#include "visual.h"
#include "pExchange.h"
#include "ideStructurePack.h"

namespace nechto::ide
{
	class drawableBlock : public sf::Drawable
	{
	public:
		rect frame;
		static i64 numberOfDraBlock;

		float illuminationThinkness = 0;
		ui32 frameColor = 4;
		ui32 illuminationColor = 0;
		vnShape::shapeType shape = vnShape::rectanleShape;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			geometricShape gShape = vnShape::getShape(shape);
			sf::ConvexShape nShape(gShape.size());
			nShape.setFillColor(col::blockColor[frameColor]);
			for (int i = 0; i < gShape.size(); ++i)
			{
				float x = gShape[i].x * frame.size.x;
				float y = gShape[i].y * frame.size.y;
				glm::vec2 pointPosition(glm::vec2(x, y) + frame.center());
				nShape.setPoint(i, GLM_SFML(pointPosition));
			}
			nShape.setOutlineColor(col::illuminationColor[illuminationColor]);
			nShape.setOutlineThickness(illuminationThinkness);

			target.draw(nShape);
		}
		drawableBlock(rect fr, ui32 frCol = 5, float ilThin = 0, 
			ui32 ilCol = 0)
			:frame(fr), frameColor(frCol),
			illuminationThinkness(ilThin), illuminationColor(ilCol)
		{}
		~drawableBlock() {--numberOfDraBlock;}
	protected:
		drawableBlock() { ++numberOfDraBlock; }//!!!опасно
	};
	i64 drawableBlock::numberOfDraBlock = 0;
	class drawableTextBlock : public drawableBlock
	{
		std::wstring blockText;
		const sf::Font& font;
		ui32 textColor = 7;
		float charSize = 18.;
	public:
		drawableTextBlock(const drawableBlock& block, const std::wstring& text,
			const sf::Font f, ui32 tCol = 7, float chSize = 18.)
			:drawableBlock(block), blockText(text), font(f), 
			charSize(chSize), textColor(tCol){}

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			drawableBlock::draw(target, states);
			sf::Text text(blockText, font, charSize);
			text.setStyle(1);
			text.setPosition(0, 0);
			text.setFillColor(col::illuminationColor[textColor]);
			//////////////////////////////////////////////////////////////////
			
			glm::vec2 size = textSize(text);
			text.setPosition(sf::Vector2f(
				frame.position.x + charSize / 4,
				frame.position.y + charSize / 6));
			target.draw(text);
		}
		void setSizeByText()
		{
			sf::Text text(blockText, font, charSize);
			glm::vec2 size = textSize(text);

			frame.size.x = size.x + charSize / 2;
			frame.size.y = size.y + charSize / 3;
		}

		friend bool operator<<(nodePtr node, const drawableTextBlock& drablock)
		{
			nodePtr pos = sPack::draBlock::frame / sPack::rect::position / node;
			nodePtr size = sPack::draBlock::frame / sPack::rect::size / node;
			if (!(
				sPack::vec2::x / pos << drablock.frame.position.x &&
				sPack::vec2::y / pos << drablock.frame.position.y &&
				sPack::vec2::x / size << drablock.frame.size.x &&
				sPack::vec2::y / size << drablock.frame.size.y))
				return false;

			sPack::draBlock::charSize / node << drablock.charSize;
			sPack::draBlock::blockColor / node << drablock.frameColor;
			sPack::draBlock::illuminationColor / node << drablock.illuminationColor;
			sPack::draBlock::textColor / node << drablock.textColor;
			
			sPack::draBlock::text / node << drablock.blockText;
			sPack::draBlock::illuminationThinkness / node << drablock.illuminationThinkness;
			return true;
		}
		friend bool operator>>(nodePtr node, drawableTextBlock& drablock)
		{
			nodePtr pos =  sPack::draBlock::frame / sPack::rect::position / node;
			nodePtr size = sPack::draBlock::frame / sPack::rect::size / node;
			if (!(
				sPack::vec2::x / pos >> drablock.frame.position.x &&
				sPack::vec2::y / pos >> drablock.frame.position.y &&
				sPack::vec2::x / size >> drablock.frame.size.x &&
				sPack::vec2::y / size >> drablock.frame.size.y))
				return false;

			sPack::draBlock::charSize / node << drablock.charSize;
			sPack::draBlock::blockColor / node >> drablock.frameColor;
			sPack::draBlock::illuminationColor / node >> drablock.illuminationColor;
			sPack::draBlock::textColor / node >> drablock.textColor;


			sPack::draBlock::text / node >> drablock.blockText;
			sPack::draBlock::illuminationThinkness / node >> drablock.illuminationThinkness;
			return true;
		}
		drawableTextBlock(nodePtr node, const sf::Font& f)
			:font(f)
		{
			assert(node >> *this);
		}
	};
}
#pragma once
#include "drawableBlock.h"
#include "entity.h"

namespace nechto::ide
{
	//отрисовываема€ сущность: отображаемый элемент, 
	//не €вл€ющийс€ vNode, vConnection или vGroup
	//нужен, чтобы отобразить что-то нестандартное
	class drawableEntity : public singleConnectedEntity, sf::Drawable
	{
	public:
		drawableEntity(entityPtr esNode)
			:singleConnectedEntity(esNode) {}
		drawableEntity()
			:singleConnectedEntity() {}
	};
	class textMark : public drawableEntity
	{
	public:
		const sf::Font& font;
		textMark(const sf::Font& f, serialPlan data = serialPlan(nullptr))
			:drawableEntity(), font(f)
		{
			eConnect(creator::createEntity(entityT::singleConnection));
			fabricate(sPack::textMark::getPlan(data));
		}
		void setVNode(nodePtr vNode)
		{
			NumHubConnect(-sPack::textMark::vNode / node(), vNode,
				sPack::textMark::vNode.last());
		}
		void setBegin(ui32 number)
		{
			sPack::textMark::beginNumber / node() << number;
		}
		ui32 getBegin() const
		{
			ui32 number = 0;
			sPack::textMark::beginNumber / node() >> number;
			return number;
		}
		void setEnd(ui32 number)
		{
			sPack::textMark::endNumber / node() << number;
		}
		ui32 getEnd() const
		{
			ui32 number = 0;
			sPack::textMark::endNumber / node() >> number;
			return number;
		}
	};
	class textSelection : public textMark
	{
	public:
		textSelection(const sf::Font& f)
			:textMark(f)
		{}
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			nodePtr draBlockNode = sPack::textMark::vNode / sPack::vNode::drawableBlock / node();
			if (!+ draBlockNode)
				return;
			drawableTextBlock draBlock(draBlockNode, font);
			ui32 begin = getBegin();
			ui32 end = getEnd();
			if (begin < end || begin < 0 || end > draBlock.blockText.size())
				return;
			sf::Text text = draBlock.getText();
			glm::vec2 charPos = GLM_SFML(text.findCharacterPos(begin));
			glm::vec2 beginPos = charPos;
			for (ui32 i = begin; i < end; ++i)
			{
				glm::vec2 nextCharPos = GLM_SFML(text.findCharacterPos(begin + 1));
				if (draBlock.blockText[i] == L'\n')
				{
					target.draw(selectedStringRectangle(
						beginPos, glm::vec2(charPos.x, text.getCharacterSize())), states);
					beginPos = nextCharPos;
				}
				charPos = nextCharPos;
			}
			target.draw(selectedStringRectangle(
				beginPos, glm::vec2(charPos.x, text.getCharacterSize())), states);
		}
	private:
		sf::RectangleShape selectedStringRectangle(glm::vec2 beginPos, glm::vec2 size) const
		{
			sf::RectangleShape rectangle;
			rectangle.setPosition(GLM_SFML(beginPos));
			rectangle.setSize(GLM_SFML(size));
			rectangle.setFillColor(color(0x004F8B33));
			return rectangle;
		}
	};
	//каретка. ѕереименовать
	class textCarriage : public textMark
	{
	public:
		textCarriage(const sf::Font& f)
			:textMark(f)
		{}
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			nodePtr draBlockNode = sPack::textMark::vNode / sPack::vNode::drawableBlock / node();
			if (!+ draBlockNode)
				return;
			drawableTextBlock draBlock(draBlockNode, font);
			ui32 number = getBegin();
			if (number < 0 || number > draBlock.blockText.size())
				return;
			sf::Text text = draBlock.getText();
			auto pos = text.findCharacterPos(number);
			sf::RectangleShape r;
			r.setPosition(pos);
			r.setSize(sf::Vector2f(2, text.getCharacterSize()));
			r.setFillColor(sf::Color::Red);
			target.draw(r, states);
		}
	private:
	};
}
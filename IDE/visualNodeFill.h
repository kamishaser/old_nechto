#pragma once
#include "visualNode.h"
#include "textOut.h"
#include "mathOperator.h"
#include "nodeOperator.h"
#include "externalObject.h"
#include "text.h"
#include "method.h"
#include "buttonList.h"

namespace nechto::ide
{
	namespace vnText
	{
		void fill(visualNode* vn1)
		{
			nodePtr n1 = vn1->get()->connection[0];
			assert(n1.exist());
			char subtype = n1->getSubtype();
			assert(n1.exist());
			switch (n1->getType())
			{
			case node::Deleted:
				vn1->nodeText = u"error:\ndeleted\n" + to_string(n1);
				break;
			case node::Hub:
				vn1->nodeText = u"error:\nhub\n" + to_string(n1);
				break;
			case node::Variable:
				if (subtype)
					vn1->nodeText = std::to_u16string(n1->getData<i64>());
				else
					vn1->nodeText = std::to_u16string(n1->getData<f64>());
				break;
			case node::MathOperator:
				vn1->nodeText = 
					typeName::mathOperatorShortT[n1->getSubtype()];
				break;
			case node::Text:
				vn1->nodeText = text::get(n1);
				break;
			case node::ConditionalBranching:
				vn1->nodeText = u"if";
				break;
			case node::Method:
				vn1->nodeText = method::getMethodName(n1);
			case node::Pointer:
				if(subtype == pointer::Simple)
					vn1->nodeText = u"*s";
				else if (subtype == pointer::ConIter)
					vn1->nodeText = u"*c";
				else
					vn1->nodeText = u"*g";
				break;
			case node::Group:
				vn1->nodeText = u"group";
				break;
			case node::ExternalObject:
				if (n1->getData<externalObject*>() == nullptr)
					vn1->nodeText = u"nullptr";
				else
					vn1->nodeText =
					n1->getData<externalObject*>()->getTypeName();
				
				break;
			case node::NodeOperator:
				vn1->nodeText =
					nodeOperator.getName(subtype);
			default:
				vn1->nodeText = u"недоделал";
			}
		}
	}
	namespace vnShape
	{
		void fill(visualNode* vn)
		{
			nodePtr n1 = vn->get()->connection[0];
			switch (n1->getType())
			{
			case node::ConditionalBranching:
			case node::Group:
				vn->nShape = rhombe();
				break;
			case node::MathOperator:
			case node::NodeOperator:
				vn->nShape = circle();
				break;
			case node::Variable:
			case node::Pointer:
				vn->nShape = octagon();
				break;
			default:
				vn->nShape = rectangle();
				break;
			}
		}
	}
	namespace vnLightColor
	{
		static const color errorColor = sf::Color::Red;
		static const color warningColor = sf::Color::Yellow;
		static const color selectedColor = sf::Color(0, 0, 255);
		static const color mOverColor = sf::Color(100, 200, 255);
		int setByAttribute(visualNode* vNode);
		int setByButtonState(sharedButton* button);
		void fill(visualNode* vNode)
		{

			int aColor = -2;
			int bColor = -2;

			assert(vNode->getTypeName() == u"nechtoIde.visualNode");
			assert(vNode->get().exist());
			auto button = sharedButton::getByNode(vNode->getConnection(0));
			if (button)
				bColor = setByButtonState(button);
			if (vNode->get()->hasHub())
				aColor = setByAttribute(vNode);
			int color = (aColor > bColor) ? aColor : bColor;
			if (color >-2)
				vNode->lightColor = col::sel[color+1];
			else vNode->lightColor = sf::Color(0, 0, 0);
		}
		int setByAttribute(visualNode* vNode)
		{
			int color = -2;
			connectionIterator ci(vNode->get());
			do
			{
				nodePtr temp = ci.get();
				if (typeCompare(temp, node::Group))
					temp = temp->connection[0];
				namedExCon* attribute = namedExCon::getByNode(temp);
				if (attribute)
				{
					//выделение
					if (attribute->name == u"mouseCursor")
						color = 4;
					else if (attribute->name == u"groupOfSelected" &&
						color < -1)
						color = -1;
					else if (attribute->name == u"lastSelected" &&
						color < 4)
					{
						for (int i = 0; i < 4; ++i)
							if (attribute->getConnection(i) == vNode->get())
							{
								if (color < i)
									color = i;
								break;
							}
					}
					//нажатия кнопок
					else if (attribute->name == u"pressedButton")
						color = 3;
				}
			} while (ci.stepForward());
			return color;
		}
		int setByButtonState(sharedButton* button)
		{
			const int baseColor = -2;
			auto bList = buttonList::getByNode(button->getList());
			if(bList && bList->isLClicked(button))
				return 2;
			return baseColor;
		}
	}
}

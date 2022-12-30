#pragma once
#include "visualNode.h"
#include "textOut.h"
#include "mathOperator.h"
#include "Object.h"
#include "text.h"
#include "method.h"
#include "buttonList.h"

namespace nechto::ide
{
	namespace vnText
	{
		void fill(visualNode* vn1)
		{
			nodePtr n1 = vn1->node().connection(0);
			assert(n1.exist());
			char subtype = n1.subtype();
			assert(n1.exist());
			switch (n1.type())
			{
			case nodeT::Deleted:
				vn1->nodeText = L"error:\ndeleted\n" + to_string(n1);
				break;
			case nodeT::Hub:
				vn1->nodeText = L"error:\nhub\n" + to_string(n1);
				break;
			case nodeT::Variable:
				if (subtype)
					vn1->nodeText = std::to_wstring(i64VariablePtr(n1).get());
				else
					vn1->nodeText = std::to_wstring(f64VariablePtr(n1).get());
				break;
			case nodeT::MathOperator:
				vn1->nodeText = 
					typeName::getMathOperatorShortSubtypeName(n1.subtype());
				break;
			case nodeT::Text:
				vn1->nodeText = textPtr(n1);
				break;
			case nodeT::Condition:
				vn1->nodeText = L"if";
				break;
			case nodeT::Method:
				vn1->nodeText = L"не доделал имена методов";
			case nodeT::Pointer:
				if(subtype == pointerT::Simple)
					vn1->nodeText = L"*s";
				else if (subtype == pointerT::PortIter)
					vn1->nodeText = L"*p";
				else
					vn1->nodeText = L"*g";
				break;
			case nodeT::Group:
				vn1->nodeText = L"group";
				break;
			case nodeT::Object:
				if (nonTypedObjectPtr(n1).getObjectPtr() == nullptr)
					vn1->nodeText = L"nullptr";
				else
					vn1->nodeText =
					nonTypedObjectPtr(n1).getObjectPtr()->getTypeName();
				
				break;
			default:
				vn1->nodeText = L"недоделал";
			}
		}
	}
	namespace vnShape
	{
		void fill(visualNode* vn)
		{
			nodePtr n1 = vn->node().connection(0);
			switch (n1.type())
			{
			case nodeT::Condition:
			case nodeT::Group:
				vn->nShape = rhombe();
				break;
			case nodeT::MathOperator:
			case nodeT::NodeOperator:
				vn->nShape = circle();
				break;
			case nodeT::Variable:
			case nodeT::Pointer:
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

			assert(vNode->getTypeName() == L"nechtoIde.visualNode");
			assert(vNode->node().exist());
			auto button = getObject<sharedButton>(vNode->node().connection(0));
			if (button)
				bColor = setByButtonState(button);
			if (vNode->node().hub().exist())
				aColor = setByAttribute(vNode);
			int color = (aColor > bColor) ? aColor : bColor;
			if (color >-2)
				vNode->lightColor = col::sel[color+1];
			else vNode->lightColor = sf::Color(0, 0, 0);
		}
		int setByAttribute(visualNode* vNode)
		{
			int color = -2;
			portIterator ci(vNode->node());
			do
			{
				nodePtr temp = ci.get();
				if (typeCompare(temp, nodeT::Group))
					temp = temp.connection(0);
				namedExCon* attribute = getObject<namedExCon>(temp);
				if (attribute)
				{
					//выделение
					if (attribute->name == L"mouseCursor")
						color = 4;
					else if (attribute->name == L"groupOfSelected" &&
						color < -1)
						color = -1;
					else if (attribute->name == L"lastSelected" &&
						color < 4)
					{
						for (int i = 0; i < 4; ++i)
							if (attribute->node().connection(i) == vNode->node())
							{
								if (color < i)
									color = i;
								break;
							}
					}
					//нажатия кнопок
					else if (attribute->name == L"pressedButton")
						color = 3;
				}
			} while (ci.stepForward());
			return color;
		}
		int setByButtonState(sharedButton* button)
		{
			const int baseColor = -2;
			auto bList = getObject<buttonList>(button->getList());
			if(bList && bList->isLClicked(button))
				return 2;
			return baseColor;
		}
	}
}

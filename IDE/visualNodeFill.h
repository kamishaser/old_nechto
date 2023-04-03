#pragma once
#include "visual.h"
#include "ideStructurePack.h"
#include "textOut.h"
namespace nechto::ide
{
	namespace vnText
	{
		const std::wstring fill(nodePtr node)
		{
			std::wstring text;
			if (!node.exist())
				text = L"nullNode";
			char subtype = node.subtype();
			switch (node.type())
			{
			case nodeT::Deleted:
				text = L"error:\ndeleted\n" + to_string(node);
				break;
			case nodeT::Hub:
				text = L"error:\nhub\n" + to_string(node);
				break;
			case nodeT::Variable:
				if (subtype)
					text = std::to_wstring(i64VariablePtr(node).get());
				else
					text = std::to_wstring(f64VariablePtr(node).get());
				break;
			case nodeT::Struct:
				fillStructData(node, text);
			case nodeT::MathOperation:
				text =
					typeName::getMathOperationShortSubtypeName(subtype);
				break;
			case nodeT::Text:
				text = textPtr(node);
				break;
			case nodeT::Condition:
				text = L"if";
				break;
			case nodeT::Pointer:
				if (subtype == pointerT::PortPointer)
					text = L"*p";
				else
					text = L"*g";
				break;
			case nodeT::Group:
				text = L"group";
				break;
			case nodeT::Entity:
				if (!entityPtr(node).entityExist())
					text = L"nullptr";
				else
					text =
					entityPtr(node).getEntityPtr()->getTypeName();
				break;
			default:
				text = typeName::getTypeAndSubtypeName(node.type(), subtype);
			}
		}
		void fillStructData(structPtr node, std::wstring& text)
		{
			structData data = node.getStructData();
			text = L"struct\n";
			for (int i = 0; i < 4; ++i)
			{
				text.push_back(data.strenght.port[i] ? L's' : L'w');
			}
		}
	}
	namespace vnShape
	{
		shapeType fill(nodePtr node)
		{
			switch (node.type())
			{
			case nodeT::Condition:
			case nodeT::Group:
				return shapeType::rhombeShape;
				break;
			case nodeT::MathOperation:
				return shapeType::circleShape;
				break;
			case nodeT::Variable:
			case nodeT::Pointer:
				return shapeType::octagonShape;
				break;
			default:
				return shapeType::rectanleShape;
				break;
			}
		}
	}
	//namespace vnIlluminationColor
	//{
	//	int setByAttribute(visualNode* vNode);
	//	int setByButtonState(sharedButton* button);
	//	void fill(visualNode* vNode)
	//	{

	//		int aColor = -2;
	//		int bColor = -2;

	//		assert(vNode->getTypeName() == L"nechtoIde.visualNode");
	//		assert(vNode->node().exist());
	//		auto button = getEntity<sharedButton>(vNode->node().connection(0));
	//		if (button)
	//			bColor = setByButtonState(button);
	//		if (vNode->node().hub().exist())
	//			aColor = setByAttribute(vNode);
	//		int color = (aColor > bColor) ? aColor : bColor;
	//		if (color >-2)
	//			vNode->lightColor = col::sel[color+1];
	//		else vNode->lightColor = sf::Color(0, 0, 0);
	//	}
	//	int setByAttribute(visualNode* vNode)
	//	{
	//		int color = -2;
	//		portPointer ci(vNode->node());
	//		do
	//		{
	//			nodePtr temp = ci.get();
	//			if (typeCompare(temp, nodeT::Group))
	//				temp = temp.connection(0);
	//			singleConnectedEntity* attribute = getEntity<singleConnectedEntity>(temp);
	//			if (attribute)
	//			{
	//				//выделение
	//				if (attribute->name == L"mouseCursor")
	//					color = 4;
	//				else if (attribute->name == L"groupOfSelected" &&
	//					color < -1)
	//					color = -1;
	//				else if (attribute->name == L"lastSelected" &&
	//					color < 4)
	//				{
	//					for (int i = 0; i < 4; ++i)
	//						if (attribute->node().connection(i) == vNode->node())
	//						{
	//							if (color < i)
	//								color = i;
	//							break;
	//						}
	//				}
	//				//нажатия кнопок
	//				else if (attribute->name == L"pressedButton")
	//					color = 3;
	//			}
	//		} while (ci.stepForward());
	//		return color;
	//	}
	//	int setByButtonState(sharedButton* button)
	//	{
	//		const int baseColor = -2;
	//		auto bList = getEntity<buttonList>(button->getList());
	//		if(bList && bList->isLClicked(button))
	//			return 2;
	//		return baseColor;
	//	}
	//}
}

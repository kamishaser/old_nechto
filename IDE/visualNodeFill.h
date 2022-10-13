#pragma once
#include "visualNode.h"
#include "textOut.h"
#include "mathOperator.h"
#include "nodeOperator.h"
#include "externalObject.h"
#include "text.h"
#include "method.h"

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
				vn1->nodeText = L"error:\ndeleted\n" + to_string(n1);
				break;
			case node::Hub:
				vn1->nodeText = L"error:\nhub\n" + to_string(n1);
				break;
			case node::Variable:
				if (subtype)
					vn1->nodeText = std::to_wstring(n1->getData<i64>());
				else
					vn1->nodeText = std::to_wstring(n1->getData<f64>());
				break;
			case node::MathOperator:
				vn1->nodeText = 
					typeName::mathOperatorShortT[n1->getSubtype()];
				break;
			case node::Text:
				vn1->nodeText = text::get(n1);
				break;
			case node::ConditionalBranching:
				vn1->nodeText = L"if";
				break;
			case node::Method:
				vn1->nodeText = method::getMethodName(n1);
			case node::Pointer:
				if(subtype == pointer::Simple)
					vn1->nodeText = L"*s";
				else if (subtype == pointer::ConIter)
					vn1->nodeText = L"*c";
				else
					vn1->nodeText = L"*g";
				break;
			case node::Group:
				vn1->nodeText = L"group";
				break;
			case node::ExternalObject:
				if (n1->getData<externalObject*>() == nullptr)
					vn1->nodeText = L"nullptr";
				else
					vn1->nodeText =
					n1->getData<externalObject*>()->getTypeName();
				
				break;
			case node::NodeOperator:
				vn1->nodeText =
					nodeOperator.getName(subtype);
			default:
				vn1->nodeText = L"недоделал";
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

		void fill(visualNode* vNode)
		{

			int selStatus = -2;

			assert(vNode->getTypeName() == L"nechtoIde.visualNode");
			assert(vNode->get().exist());
			if (vNode->get()->hasHub())
			{
				connectionIterator ci(vNode->get());
				do
				{
					nodePtr temp = ci.get();
					if (typeCompare(temp, node::Group))
						temp = temp->connection[0];
					namedExCon* attribute = namedExCon::getByNode(temp);
					if (attribute)
					{

						if (attribute->name == L"mouseCursor")
							selStatus = 4;
						if (attribute->name == L"groupOfSelected")
							if (selStatus < -1)
								selStatus = -1;
						if (attribute->name == L"lastSelected" && selStatus < 4)
							for (int i = 0; i < 4; ++i)
								if (attribute->getConnection(i) == vNode->get())
								{
									if (selStatus < i)
										selStatus = i;
									break;
								}
					}
				} while (ci.stepForward());
			}
			if (selStatus >-2)
				vNode->lightColor = col::sel[selStatus+1];
			else vNode->lightColor = sf::Color(0, 0, 0);
		}
	}
}

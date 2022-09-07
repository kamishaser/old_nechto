#pragma once
#include "visualNode.h"
#include "textOut.h"
#include "mathOperator.h"
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
				vn1->nodeText = typeName::mathOperatorShortT[n1->getSubtype()];
				break;
			case node::Text:
				vn1->nodeText = typeName::textT[n1->getSubtype()] +
					text::get(n1);
				break;
			case node::ConditionalBranching:
				vn1->nodeText = L"if";
				break;
			case node::Method:
				vn1->nodeText = method::getMethodName(n1);
			case node::Pointer:
				if(subtype == pointer::Simple)
					
				break;
			case node::Group:

				break;
			case node::ExternalObject:

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
			nodePtr n1 = vn->get()->connection[0];
			switch (n1->getType())
			{
			case node::ConditionalBranching:
			case node::Group:
				vn->nShape = rhombe();
				break;
			case node::MathOperator:
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
		static color errorColor = sf::Color::Red;
		static color warningColor = sf::Color::Yellow;
		static color highlightedColor = sf::Color::Blue;
		static color mOverColor = sf::Color(100, 200, 255);

		void fill(visualNode* vNode)
		{
			bool warng = false;
			bool error = false;
			bool mOver = false;
			bool highl = false;

			assert(vNode->getTypeName() == L"nechtoIde.visualNode");
			color temp;
			assert(vNode->get().exist());
			if (vNode->get()->hasHub())
			{
				connectionIterator i(vNode->get());
				do
				{

					namedExCon* attribute = namedExCon::getByNode(i.get());
					if (attribute)
					{
						
						if (attribute->name == L"#nechtoIde.cursored")
							mOver = true;
						if (attribute->name == L"#nechtoIde.error")
							error = true;
						if (attribute->name == L"#nechtoIde.warning")
							warng = true;
					}
				} while (i.stepForward());
			}
			color ew =
				(error) ? errorColor :
				(warng) ? warningColor :
				sf::Color(0, 0, 0);
			color mh =
				(mOver) ? mOverColor :
				(highl) ? highlightedColor :
				sf::Color(0, 0, 0);

			vNode->lightColor = ew + mh;
		}
	}
}

#pragma once
#include "visualNode.h"
#include "namedExCon.h"

namespace nechto::ide
{
	class textInputBox : public namedExCon
	{
		const int deleteKey = 8;
		const int enterKey = 13;
		const int escapeKey = 27;
		const int controlV = 22;
		bool Focus = false;
	public:
		std::wstring* stringAim = nullptr;
		
		i64 limit = 1000;
		std::wstring header = L"ввод текста";
		std::wstring iText;

		textInputBox(nodePtr v1)
			:namedExCon(L"TextInputBox")
		{
			NumHubConnect(get(), v1, 0);
		}
		visualNode* aim() const
		{
			return visualNode::getByNode(getConnection(1));
		}
		visualNode* box() const
		{
			return visualNode::getByNode(getConnection(0));
		}
		void update(int input)
		{
			if (Focus)
			{
				if (input == deleteKey)
					backSpace();
				else if (input == enterKey)
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
						iText.push_back(L'\n');
					else
						reset();
				else if (input == escapeKey)
					reset();
				else if (input == controlV)
					iText += sf::Clipboard::getString();
				else
					iText.push_back(static_cast<wchar_t>(input));
				
				if (iText.size() > limit)
						iText.resize(limit);
			}
			updateBox();
			updateAim();
		}
		void focus(std::wstring* s = nullptr)
		{
			Focus = true;
			stringAim = s;
		}
		bool hasFocus()
		{
			return Focus;
		}
		void reset()
		{
			if (hasFocus())
			{
				Focus = false;
				
				if (stringAim != nullptr)
				{
					*stringAim = iText;
					stringAim = nullptr;
				}
				else if (aim())
				{
					aim()->frame.size = glm::vec2(0, 0);
					aim()->nodeText = iText;
				}
			}
		}
	private:
		void updateBox()
		{
			box()->nodeText = header +
				std::wstring((hasFocus()) ? L" *\n" : L" -\n") +
				iText;
		}
		void updateAim()
		{
			if (aim())
			{
				nodePtr v1 = aim()->getConnection(0);
				aim()->nodeText = iText + L'|';
				if (typeCompare(v1, node::Text))
					text::set(v1, iText);
			}
		}
		void backSpace()
		{
			if (!iText.empty())
				iText.pop_back();
		}
	};
}
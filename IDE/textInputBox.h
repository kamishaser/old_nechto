#pragma once
#include "visualNode.h"
#include "namedExCon.h"
#include "button.h"

namespace nechto::ide
{
	class textInputBox : public namedExCon
	{
		const int deleteKey = 8;
		const int enterKey = 13;
		const int escapeKey = 27;
		const int controlV = 22;
		bool Focus = false;
		bool reseted = false;
	public:
		int numberOfPoint = 0;

		

		bool checkCorectness() const
		{
			nodePtr v1;
			if (aim())
				v1 = aim()->getConnection(0);
			if (!v1.exist())
				return false;
			switch (v1->getType())
			{
			case node::Variable:
				if ((iText.size() > 12) || iText.empty())
					return false;
				if (iText == L"-")
					return false;
				if (v1->getSubtype())//i64
					return isI64(iText);
				else
					return isF64(iText);
				break;
			case node::Text:
				return true;
			default:
				return false;
			}
		}
		
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
		void focus(visualNode* vNode)
		{
			reset();
			Focus = true;
			iText.clear();
			nodePtr v1 = vNode->getConnection(0);
			if (vNode && v1.exist() && 
				(typeCompare(v1, node::Text) || typeCompare(v1, node::Variable)))
			{
				switch (v1->getType())
				{
				case node::Text:
					iText = text::get(v1);
					break;
				case node::Variable:
					if (v1->getSubtype())
						iText = std::to_wstring(v1->getData<i64>());
					else
						iText = std::to_wstring(v1->getData<f64>());
					break;
				default:
					return;
				}
				NumHubConnect(get(), vNode->get(), 1);
			}
		}
		bool hasFocus()
		{
			return Focus;
		}
		bool reset()
		{
			if (hasFocus())
			{
				Focus = false;
				reseted = true;
				if (aim())
				{
					return setAimData();
				}
			}
			numDisconnect(get(), 1);
			return false;
		}
		bool boxResetEvent()
		{
			if (reseted)
			{
				reseted = false;
				return true;
			}
			return false;
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
			setAimData();
		}
		void backSpace()
		{
			if (!iText.empty())
				iText.pop_back();
		}
		bool setAimData()
		{
			if (aim())
			{
				aim()->frame.size = glm::vec2(0, 0);
				nodePtr v1 = aim()->getConnection(0);
				if (checkCorectness())
				{
					aim()->nodeText = iText;
					if (v1.exist())
						setNodeData(v1);
					return true;
				}
				else
				{
					aim()->nodeText = L"ERROR";
					if (!iText.empty() && !(isPoint(iText.back()) || isMinus(iText.back())))
						backSpace();
				}
			}
			return false;
		}
		void setNodeData(nodePtr v1)
		{
			switch (v1->getType())
			{
			case node::Variable:
				if (iText.empty())
					return;
				if (v1->getSubtype())//i64
					v1->setData<i64>(std::stoll(iText));
				else
					v1->setData<f64>(std::stod(iText));
				break;
			case node::Text:
				text::set(v1, iText);
				break;
			}
		}

		static bool isPoint(wchar_t ch)
		{
			return (ch == L'.' || ch == L',');
		}
		static bool isDigit(wchar_t ch)
		{
			return (ch >= L'0' && ch <= L'9');
		}
		static bool isMinus(wchar_t ch)
		{
			return ch == L'-';
		}
		
		static bool isI64(const std::wstring& text)
		{
			if (text.empty())
				return false;
			for (size_t i = 0; i < text.size(); ++i)
			{
				if(!isDigit(text[i]))
					if(!(isMinus(text[i]) && i == 0))
						return false;
			}
			return true;
		}

		static bool isF64(const std::wstring& text)
		{
			if (text.empty())
				return false;
			int pointCounter = 0;
			for (size_t i = 0; i < text.size(); ++i)
			{
				if (isDigit(text[i]))
					continue;
				if (isPoint(text[i]))
				{
					if (i == 0 || i == text.size() - 1)
						return false;
					++pointCounter;
					continue;
				}
				if (isMinus(text[i]) && i == 0)
					continue;
				return false;
			}
			if (pointCounter > 1)
				return false;
			return true;
		}
	};
}
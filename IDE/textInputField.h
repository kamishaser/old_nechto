#pragma once
#include "keyboard.h"

namespace nechto::ide
{
	class textInputField : public singleConnectedEntity
	{
		const wchar_t deleteKey = 8;
		const wchar_t enterKey = 13;
		const wchar_t escapeKey = 27;
		const wchar_t controlV = 22;

		i64 limit = 1000;
		std::wstring iText;
	public:
		textInputField()
		{
			eConnect(creator::createEntity(entityT::singleConnection));
			//fabricate(sPack::interactiveHandler::getPlan(), node());

		}
		void finalConnect(nodePtr ideNode)
		{
			
		}
		void update()
		{
			while (!windowEntity::inputTextEvents.empty())
			{
				updateInput(windowEntity::inputTextEvents.front());
				windowEntity::inputTextEvents.pop();
			}
		}
	private:
		void updateInput(wchar_t input)
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
				iText.push_back(input);

			if (iText.size() > limit)
				iText.resize(limit);
			std::wcout << iText << std::endl;
		}
		void backSpace()
		{
			if (!iText.empty())
				iText.pop_back();
		}
		void reset()
		{
			//запуск события reset
		}
	};
}
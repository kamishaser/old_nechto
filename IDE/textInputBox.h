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
		void focus()
		{
			Focus = true;
		}
		bool hasFocus()
		{
			return Focus;
		}
		void reset()
		{
			Focus = false;
			aim()->frame.size = glm::vec2(0, 0);
			aim()->nodeText = iText;
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
/*
Сие есть первая версия ide
в которой есть возможность
не использую консоль

вводить текст

редактировать граф

к сожалению пока что
возможность сильно ограничены.
Например:

можно работать только
с текстовыми нодами

нельзя перемещать камеру


В ближайшее время я планирую добавить

добавление и редактирования
нод разных типов

полноценное
отображение
соединений

перемещение камеры

несколько рабочих окон

файловое сохранений
(уже делал, надо
только доработать)


А потом всё опять сломаю.
И похоже надолго.

Надо:

1) много чего переделать

2) отрефакторить говонокод

3) перераспихать 
код по файлам

4) возможно перевести
на модули с хэдэров

5) писать тесты

6) и много чего по мелочи


А затем займусь полноценной
разработкой редактора nechto
с помощью nechto

В общем будет весело
*/
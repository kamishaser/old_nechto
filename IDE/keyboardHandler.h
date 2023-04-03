#pragma once
//#include "display.h"
//#include "button.h"
//
//namespace nechto::ide
//{
//	class keyboardHandler
//	{
//	public:
//		std::array<basicButton, sf::Keyboard::KeyCount> keyBoardKeys;
//		bool textMode = false;
//		using key = sf::Keyboard::Key;
//		
//		void update()
//		{
//			for (int i = 0; i < keyBoardKeys.size(); ++i)
//				keyBoardKeys[i].update(sf::Keyboard::isKeyPressed(static_cast<key>(i)));
//		}
//		bool control() const
//		{
//			return	keyBoardKeys[static_cast<int>(key::LControl)].isPressed() ||
//				keyBoardKeys[static_cast<int>(key::RControl)].isPressed();
//		}
//		bool shift() const
//		{
//			return	keyBoardKeys[static_cast<int>(key::LShift)].isPressed() ||
//				keyBoardKeys[static_cast<int>(key::RShift)].isPressed();
//		}
//		bool alt() const
//		{
//			return	keyBoardKeys[static_cast<int>(key::LAlt)].isPressed() ||
//				keyBoardKeys[static_cast<int>(key::RAlt)].isPressed();
//		}
//
//		basicButton& operator[](key k)
//		{
//			return keyBoardKeys[static_cast<int>(k)];
//		}
//	};
//}
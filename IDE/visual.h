#pragma once
#include "GLM/glm.hpp"
#include "SFML/Graphics.hpp"
//#include "nodeOperationSet.h"
//#include "connectionPointer.h"
//#include "entity.h"
#include "rect.h"
//
//#include <codecvt>
//#include <locale>
//
namespace nechto::ide
{
	template<class T = float>
	sf::Vector2<T> GLM_SFML(glm::vec2 v)
	{
		return sf::Vector2<T>(v.x, v.y);
	}
	template<class T>
	glm::vec2 GLM_SFML(sf::Vector2<T> v)
	{
		return glm::vec2(v.x, v.y);
	}
	std::wstring to_string(glm::vec2 vec)
	{
		return static_cast<std::wstring>(L"[") +
			std::to_wstring(vec.x) + L' ' +
			std::to_wstring(vec.y) + L']';
	}
	using color = sf::Color;

	glm::vec2 textSize(const sf::Text text)
	{
		auto bounds = text.getLocalBounds();
		return glm::vec2(bounds.width, bounds.height);
	}
	glm::vec2 textSize(const std::wstring& string, const sf::Font font, float size)
	{
		sf::Text text(string, font, size);
		return textSize(text);
	}
	glm::vec2 randomOffset(float max)
	{
		return glm::vec2
		(
			static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / max / 2)) - max,
			static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / max / 2)) - max
		);
	}
	using namespace std::chrono;

	milliseconds currentTime()
	{
		return duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	}
	

	using geometricShape = std::vector<glm::vec2>;
//	
	namespace col
	{
		enum ilColorEnum
		{
			None,
			Error,
			Tip0,
			Tip1,
			Tip2,
			Sel4,
			Sel3,
			Sel2,
			Sel1,
			Sel0,
			Mouse
		};
		color illuminationColor[]
		{
			color(0x00000000), //none
			color(0x7CC800FF), //error
			color(0xB3EF53FF), //tip0
			color(0xCDEF96FF), //tip1
			color(0xB30058FF), //tip2
			color(0x002846FF), //sel4
			color(0x20415AFF), //sel3
			color(0x004F8BFF), //sel2
			color(0x4D9EDCFF), //sel1
			color(0x8BB9DCFF), //sel0
			color(0xDDDDDDFF), //mOver  
			color(0xFFFFFFFF)  //clickMOver 
		};
		color lineColor[]
		{
			color(0xB30058FF), //error
			color(0xE892BDFF), //weak
			color(0xE8519BFF), 
			//weak && (readOnly || последовательное || групповое)
			color(0x74294EFF), //strong
			color(0x5B002CFF), //strong && (readOnly || group)
		};
		color blockColor[]
		{
			color(0x00000000), //none
			color(0x3E6500FF), //backGroundColor
			color(0x61812EFF), //vNodeBackGroundColor
			color(0x6D3F00FF),
			color(0x8C6531FF),
			color(0xD97C00FF),
			color(0xF3B055FF),
			color(0xF3CD99FF)
		};

		color background = color(0x3E6500FF);
		color vGroupBackground = color(0x61812EFF);
		color tip0 = color(0x7CC800FF);
		color tip1 = color(0xB3EF53FF);
		color tip3 = color(0xCDEF96FF);
		
		color vNodeG0 = color(0xF3CD99FF);
		color vNodeG1 = color(0xF3B055FF);
		color vNodeG2 = color(0xD97C00FF);
		color vNodeG3 = color(0x8C6531FF);
		color vNodeG4 = color(0x6D3F00FF);

		color error = color(0xB30058FF);
		color strongReadOnly = color(0x5B002CFF);
		color strong = color(0x74294EFF);
		color weak = color(0xE892BDFF);
		color weakReadOnly = color(0xE8519BFF);
	}
//	
	namespace vnShape
	{
		enum shapeType
		{
			noShape,
			rectanleShape,
			rhombeShape,
			octagonShape,
			circleShape
		};
		constexpr geometricShape rectangle()
		{
			return geometricShape
			{
				glm::vec2(-0.5,-0.5),
				glm::vec2(0.5 ,-0.5),
				glm::vec2(0.5 , 0.5),
				glm::vec2(-0.5, 0.5)
			};
		}
		constexpr geometricShape  rhombe()
		{
			float size = 0.75f;
			return geometricShape
			{
				glm::vec2(-size, 0),
				glm::vec2(0, -size),
				glm::vec2(size, 0),
				glm::vec2(0, size)
			};
		}
		constexpr geometricShape  octagon()
		{
			float size = 0.25f;
			return geometricShape
			{
				glm::vec2(-0.5, -size),
				glm::vec2(-size, -0.5),
				glm::vec2(size, -0.5),
				glm::vec2(0.5, -size),
				glm::vec2(0.5, size),
				glm::vec2(size, 0.5),
				glm::vec2(-size, 0.5),
				glm::vec2(-0.5, size),
			};
		}
		constexpr geometricShape circle()
		{
			geometricShape temp(32);
			float angle = 0;
			float step = 3.141f / (32 / 2);
			for (int i = 0; i < temp.size(); ++i)
			{
				temp[i] = glm::vec2(glm::cos(angle), glm::sin(angle)) / 2.0f;
				angle += step;
			}
			return temp;
		}
		constexpr geometricShape getShape(shapeType type)
		{
			switch (type)
			{
			case nechto::ide::vnShape::rectanleShape:
				return rectangle();
			case nechto::ide::vnShape::rhombeShape:
				return rhombe();
			case nechto::ide::vnShape::octagonShape:
				return octagon();
			case nechto::ide::vnShape::circleShape:
				return circle();
			default:
				return geometricShape();
			}
		}
	}
	enum class buttonStatus
	{
		None,
		Press,
		BeginClick,
		EndClick
	};
}
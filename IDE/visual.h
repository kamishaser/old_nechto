#pragma once
#include "GLM/glm.hpp"
#include "SFML/Graphics.hpp"
#include "connectionRule.h"
#include "nodeOperationSet.h"
#include "connectionIterator.h"
#include "graphOperations.h"

namespace nechto::ide
{
	template<class T = float>
	sf::Vector2<T> GLM_SFML(glm::vec2 v)
	{
		return sf::Vector2<T>(v.x, v.y);
	}
	template<class T>
	glm::vec2 SFML_GLM(sf::Vector2<T> v)
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
	using geometricShape = std::vector<glm::vec2>;

	namespace col
	{
		color sel[]
		{
			color(0x002846FF),
			color(0x20415AFF),
			color(0x004F8BFF),
			color(0x4D9EDCFF),
			color(0x8BB9DCFF),
			color(0xFFFFFFFF)
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

	namespace vnShape
	{
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
	}
}
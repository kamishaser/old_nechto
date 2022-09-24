#pragma once
#include "GLM/glm.hpp"
#include "SFML/Graphics.hpp"
#include "connectionRule.h"
#include "nodeOperationSet.h"
#include "connectionIterator.h"
#include "graphOperations.h"

namespace nechto::ide
{
	sf::Vector2f GLM_SFML(glm::vec2 v)
	{
		return sf::Vector2f(v.x, v.y);
	}
	glm::vec2 SFML_GLM(sf::Vector2f v)
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
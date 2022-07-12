#pragma once
#include "GLM/glm.hpp"
#include "SFML/Graphics.hpp"

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
	using color = sf::Color;
	using geometricShape = std::vector<glm::vec2>;
}
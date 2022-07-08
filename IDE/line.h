#pragma once
#include "SFML/Graphics.hpp"
#include "GLM/trigonometric.hpp"
#include "GLM/gtx/rotate_vector.hpp"
#include "GLM/glm.hpp"

#include "SFML GML compatibility.h"

namespace nechto::ide
{
	class line : public sf::ConvexShape
	{
		glm::vec2 first;
		glm::vec2 second;
		float size;

		void compute()
		{
			glm::vec2 temp = second - first;
			glm::vec3 normal(temp.x, temp.y, 0);
			normal = glm::rotateZ(normal, 3.14f / 2);
			temp = glm::vec2(normal.x, normal.y);
			temp = glm::normalize(temp) * (size / 2);
			setPosition(0, 0);
			setPoint(0, GLM_SFML(first + temp));
			setPoint(1, GLM_SFML(first - temp));
			setPoint(2, GLM_SFML(second - temp));
			setPoint(3, GLM_SFML(second + temp));
		}
	public:
		line(glm::vec2 firstPoint, glm::vec2 secondPoint, float sizeLine)
			:first(firstPoint), second(secondPoint), size(sizeLine), sf::ConvexShape(4)
		{
			compute();
		}
		virtual ~line() {}
		void setFirst(glm::vec2 firstPoint)
		{
			first = firstPoint;
			compute();
		}
		void setSecond(glm::vec2 secondPoint)
		{
			second = secondPoint;
			compute();
		}

	};
}
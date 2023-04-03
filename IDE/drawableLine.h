#pragma once
#include "SFML/Graphics.hpp"
#include "GLM/gtx/rotate_vector.hpp"
#include "GLM/glm.hpp"

#include "visual.h"

namespace nechto::ide
{
	class drawableLine : public sf::Drawable
	{
	public:
		glm::vec2 first;
		glm::vec2 second;

		ui32 lineColor;
		float thinkness;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			////////////////////////////////////////////////////////////////////////
			sf::ConvexShape line(4);
			glm::vec2 temp = first - second;
			glm::vec3 normal(temp.x, temp.y, 0);
			normal = glm::rotateZ(normal, 3.14f / 2);
			temp = glm::vec2(normal.x, normal.y);
			temp = glm::normalize(temp) * (thinkness / 2);
			////////////////////////////////////////////////////////////////////////
			line.setPoint(0, GLM_SFML(first + temp));
			line.setPoint(1, GLM_SFML(first - temp));
			line.setPoint(2, GLM_SFML(second - temp));
			line.setPoint(3, GLM_SFML(second + temp));
			////////////////////////////////////////////////////////////////////////
			line.setFillColor(col::lineColor[lineColor]);
			////////////////////////////////////////////////////////////////////////
			target.draw(line);
		}
		drawableLine(glm::vec2 f, glm::vec2 s, ui32 lColor, float lThin)
			:first(f), second(s), lineColor(lColor), thinkness(lThin) {}

	};
}
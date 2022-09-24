#pragma once
#include "GLM/glm.hpp"

namespace nechto::ide
{
	struct rect
	{
		glm::vec2 position;
		glm::vec2 size;
		bool containsX(float x)const
		{
			return (position.x < x) && (x < (position.x + size.x));
		}
		bool containsY(float y)const
		{
			return (position.y < y) && (y < (position.y + size.y));
		}
		bool contains(glm::vec2 point)const
		{
			return containsX(point.x) && containsY(point.y);
		}
		//накладываются друг на друга
		bool intersects(const rect& of)const
		{
			//верхний правый угол второй рамки
			glm::vec2 ofTR{ of.position.x + of.size.x, of.position.y };
			//нижний левый угол второй рамки
			glm::vec2 ofBL{ of.position.x, of.position.y + of.size.y };

			return (contains(ofTR) || contains(ofBL) ||
				of.contains(position) || of.contains(position + size));
		}
		//
		bool contains(const rect& of)const
		{
			return contains(of.position) && contains(of.position + of.size);
		}
		glm::vec2 center()
		{
			return position + (size / 2.f);
		}
		void setPositionByCenter(glm::vec2 center)
		{
			position = center - (size / 2.f);
		}
		float top()
		{
			return position.y;
		}
		float bottom()
		{
			return position.y + size.y;
		}
		float left()
		{
			return position.x;
		}
		float right()
		{
			return position.x + size.x;
		}
		rect(glm::vec2 Position, glm::vec2 Size)
			:position(Position), size(Size) {}
		//получить рамку по двум любым её вершинам
		static rect getByTwoPoint(glm::vec2 first, glm::vec2 second)
		{
			glm::vec2 position{
				(first.x < second.x) ? first.x : second.x,
				(first.y < second.y) ? first.y : second.y };
			glm::vec2 size{
				((first.x > second.x) ? first.x : second.x) - position.x,
				((first.y > second.y) ? first.y : second.y) - position.y };
			return rect(position, size);
		}
	};
}

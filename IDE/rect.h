#pragma once
#include "GLM/glm.hpp"
#include "ideStructurePack.h"

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
		glm::vec2 center() const
		{
			return position + (size / 2.f);
		}
		void setPositionByCenter(glm::vec2 center)
		{
			position = center - (size / 2.f);
		}
		float top() const
		{
			return position.y;
		}
		float bottom() const
		{
			return position.y + size.y;
		}
		float left() const
		{
			return position.x;
		}
		float right() const
		{
			return position.x + size.x;
		}
		rect(glm::vec2 Position, glm::vec2 Size)
			:position(Position), size(Size) {}
		rect(float left = 0, float top = 0, float width = 0, float height = 0)
			:position{ left, top }, size{ width, height } {}
		
		//получить рамку по двум любым её противоположным вершинам
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
		friend bool operator<<(nodePtr node, const rect& r)
		{
			nodePtr pos = sPack::rect::position / node;
			nodePtr size = sPack::rect::size / node;
			if (!(
				sPack::vec2::x / pos << r.position.x &&
				sPack::vec2::y / pos << r.position.y &&
				sPack::vec2::x / size << r.size.x &&
				sPack::vec2::y / size << r.size.y))
				return false;
			return true;
		}
		friend bool operator>>(nodePtr node, rect& r)
		{
			nodePtr pos = sPack::rect::position / node;
			nodePtr size = sPack::rect::size / node;
			if (!(
				sPack::vec2::x / pos >> r.position.x &&
				sPack::vec2::y / pos >> r.position.y &&
				sPack::vec2::x / size >> r.size.x &&
				sPack::vec2::y / size >> r.size.y))
				return false;
			return true;
		}
		rect(nodePtr node)
		{
			assert(node >> *this);
		}
	};
	bool operator <<(nodePtr node, const glm::vec2& vec)
	{
		return
			sPack::vec2::x / node << vec.x &&
			sPack::vec2::y / node << vec.y;
	}
	bool operator >>(nodePtr node, glm::vec2& vec)
	{
		return
			sPack::vec2::x / node >> vec.x &&
			sPack::vec2::y / node >> vec.y;
	}
	bool operator += (nodePtr node, glm::vec2& vec)
	{
		glm::vec2 value(0, 0);
		if (!(node >> value))
			return false;
		value += vec;
		node << value;
		return true;
	}
}

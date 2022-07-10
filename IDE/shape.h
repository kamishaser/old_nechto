#pragma once
#include <vector>
#include "GLM/glm.hpp"

namespace nechto::ide
{
	class shape
	{
		std::vector<glm::vec2> points;
	public:
		shape(const std::vector<glm::vec2>& p)
			:points(p)
		{
			center();
			scale();
		}
		shape() {}
		const glm::vec2& operator[](int64_t number)
		{
			return points[number];
		}
		const int64_t size()
		{
			return points.size();
		}
		const std::vector<glm::vec2>::const_iterator begin()
		{
			return points.cbegin();
		}
		const std::vector<glm::vec2>::const_iterator end()
		{
			return points.cend();
		}
		const std::vector<glm::vec2> get()
		{
			return points;
		}

		void center()
		{
			if (points.empty())
				return;
			glm::vec2 offset(0, 0);
			for (auto i : points)
				offset += i;
			offset = offset /= points.size();
			for (auto i : points)
				i -= offset;
		}
		void scale()
		{
			bool increase = false;
			for (auto i : points)
			{
				float x = abs(i.x);
				float y = abs(i.x);
				if (x < 0.5 && y < 0.5)
				{
					increase = true;
					break;
				}
			}
			if (increase)
			{
				//минимально необходимый для выхода всех точек их квадрата 1 * 1 множитель
				float minMultiplier = 1;
				for (auto i : points)
				{
					float x = abs(i.x);
					float y = abs(i.x);
					
					if (x < 0.5 && y < 0.5)
					{
						float multiplier = 0.5f / ((x > y) ? x : y);
						if (multiplier > minMultiplier)
							minMultiplier = multiplier;
					}
				}
				for (auto i : points)
					i *= minMultiplier;
			}
			else
			{
				float maxDivisor = FLT_MAX;
				for (auto i : points)
				{
					float x = abs(i.x);
					float y = abs(i.x);
					float divisor = ((x < y) ? x : y) / 0.5f;
					if (divisor < maxDivisor)
						maxDivisor = divisor;
				}
				for (auto i : points)
					i /= maxDivisor;
			}
		}
	};
	struct locatedShape
	{
		shape elshape;
		glm::vec2 position;
		glm::vec2 scale;
	};
}

#pragma once
#include "graph.h"
#include "GLM/glm.hpp"

namespace nechto::ide::handler
{
	class spreadHandler : public graph::handler
	{
	public:
		float attractionSpeed;
		float repulsionSpeed;
		float centripetSpeed;
		glm::vec2 boardCenter;
		spreadHandler(glm::vec2 bCenter, float asp, float rsp, float csp,
			milliseconds minPeriod, milliseconds maxPeriod = 0ms)
			:graph::handler(minPeriod, maxPeriod), boardCenter(bCenter),
			attractionSpeed(asp), repulsionSpeed(rsp), centripetSpeed(csp) {}

		virtual void update()override
		{
			//перебор всех нод
			for (auto i1 = nGraph->nodes.begin(); i1 != nGraph->nodes.end(); ++i1)
			{
				//для каждой ноды сначала вычесляется смещение, а только потом применяется
				glm::vec2 offset = glm::vec2(0,0);

				//стремление к центру
				offset += centripet(i1->second);
				//отталкивание от других нод
				for (auto i2 = nGraph->nodes.begin(); i2 != nGraph->nodes.end(); ++i2)
					if (i1->first != i2->first)
						offset += repuls(i1->second, i2->second.position);

				i1->second.position += offset;
			}
		}

		glm::vec2 centripet(visualNode& v1)
		{
			float distance = glm::length(v1.position - boardCenter);
			if (distance < 1)
				return glm::vec2(0, 0);

			glm::vec2 normal = glm::normalize(v1.position - boardCenter);
			return glm::vec2
			(
				-glm::sqrt(distance) * normal.x * centripetSpeed,
				-glm::sqrt(distance) * normal.y * centripetSpeed
			);
		}
		float atScalar(float distance, float averageSize)
		{
			float scal = (distance > averageSize) ?
				(distance - averageSize) / (averageSize) : 0;
			return scal;
		}
		glm::vec2 attract(visualNode& v1, glm::vec2 pos)
		{
			float distance = glm::length(v1.position - pos);

			glm::vec2 normal = -glm::normalize(v1.position - pos);
			return glm::vec2
			(
				atScalar(distance, v1.size.x) * normal.x * attractionSpeed,
				atScalar(distance, v1.size.y) * normal.y * attractionSpeed
			);
		}
		float repScalar(float distance, float averageSize)
		{
			float scal = (1 / glm::abs(distance - averageSize));
			//if()
			return scal;
		}
		glm::vec2 repuls(visualNode& v1, glm::vec2 pos)
		{
			float distance = glm::length(v1.position - pos);

			glm::vec2 normal = glm::normalize(v1.position - pos);
			return glm::vec2
			(
				repScalar(distance, v1.size.x) * normal.x * repulsionSpeed,
				repScalar(distance, v1.size.y) * normal.y * repulsionSpeed
			);
		}
	};
}

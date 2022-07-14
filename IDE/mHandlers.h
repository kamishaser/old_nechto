#pragma once
#include "graph.h"
#include "GLM/glm.hpp"

namespace nechto::ide::handler
{

	class repulsionHandler : public graph::handler
	{
		float scalar(float distance, float averageSize)
		{
			float scal = (1 / glm::abs(distance - averageSize));
			//if()
			return scal;
		}
		glm::vec2 f(visualNode& v1, visualNode& v2)
		{
			float distance = glm::length(v1.position - v2.position);
			
			glm::vec2 normal = glm::normalize(v1.position - v2.position);
			glm::vec2 averageSize = (v1.size + v2.size) / 2.0f;
			return glm::vec2
			(
				scalar(distance, averageSize.x) * normal.x * speed,
				scalar(distance, averageSize.y) * normal.y * speed
			);
		}
	public:
		float speed;
		repulsionHandler(float sp ,milliseconds minPeriod, milliseconds maxPeriod = 0ms)
			:speed(sp), graph::handler(minPeriod, maxPeriod) {}

		virtual ~repulsionHandler() override {}

		virtual void update() override
		{
			for (auto i1 = nGraph->nodes.begin(); i1 != nGraph->nodes.end(); ++i1)
				for (auto i2 = nGraph->nodes.begin(); i2 != nGraph->nodes.end(); ++i2)
					if(i1->first != i2->first)
						i1->second.position += f(i1->second, i2->second);
		}
	};

	class attractionHandler : public graph::handler
	{
		float scalar(float distance, float averageSize)
		{
			float scal = (distance > averageSize) ?
				(distance - averageSize) / (averageSize) : 0;
			std::cout << scal << std::endl;
			return scal;
		}
		glm::vec2 f(visualNode& v1, visualNode& v2)
		{
			float distance = glm::length(v1.position - v2.position);

			glm::vec2 normal = -glm::normalize(v1.position - v2.position);
			glm::vec2 averageSize = (v1.size + v2.size) * 0.5f;
			return glm::vec2
			(
				scalar(distance, averageSize.x) * normal.x * speed,
				scalar(distance, averageSize.x) * normal.y * speed
			);
		}
	public:
		float speed;
		attractionHandler(float sp, milliseconds minPeriod, milliseconds maxPeriod = 0ms)
			:speed(sp), graph::handler(minPeriod, maxPeriod) {}

		virtual ~attractionHandler() override {}

		virtual void update() override
		{
			for (auto i1 = nGraph->connections.begin(); i1 != nGraph->connections.end(); ++i1)
				nGraph->findNode(i1->first.first).position +=
				f(nGraph->findNode(i1->first.first), nGraph->findNode(i1->first.second));
		}
	};
	class centripetalHandler : public graph::handler
	{
		glm::vec2 centripet(visualNode& v1)
		{
			float distance = glm::length(v1.position-center);
			
			glm::vec2 normal = glm::normalize(v1.position-center);
			return glm::vec2
			(
				-glm::sqrt(distance) * normal.x * speed,
				-glm::sqrt(distance) * normal.y * speed
			);
		}
	public:
		float speed;
		glm::vec2 center;
		centripetalHandler(float sp, glm::vec2 c, milliseconds minPeriod, milliseconds maxPeriod = 0ms)
			:speed(sp), center(c), graph::handler(minPeriod, maxPeriod) {}


		virtual ~centripetalHandler() override {}

		virtual void update() override
		{
			
			for (auto i1 = nGraph->nodes.begin(); i1 != nGraph->nodes.end(); ++i1)
			{
				i1->second.position += centripet(i1->second);
				
			}
		}
	};

}

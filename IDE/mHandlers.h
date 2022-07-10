#pragma once
#include "graph.h"
#include "GLM/glm.hpp"

namespace nechto::ide::handler
{

	class repulsionHandler : public graph::handler
	{
		float scalar(float distance, float averageSize)
		{
			return 1 / glm::abs(distance - averageSize / 2);
		}
		glm::vec2 f(visualNode& v1, visualNode& v2, microseconds timeInterval)
		{
			float distance = glm::length(v1.position - v2.position);
			
			glm::vec2 normal = glm::normalize(v1.position - v2.position);
			glm::vec2 averageSize = (v1.size + v2.size) * 0.5f;
			return glm::vec2
			(
				scalar(distance, averageSize.x) * normal.x * force * (timeInterval / 1ms),
				scalar(distance, averageSize.x) * normal.y * force * (timeInterval / 1ms)
			);
		}
	public:
		float force;
		
		repulsionHandler(float fc)
			:force(fc) {}

		virtual ~repulsionHandler() override {}

		virtual void update(milliseconds timeInterval) override
		{
			for (auto i1 : nGraph->nodes)
				for (auto i2 : nGraph->nodes)
					if(i1.first != i2.first)
						i1.second.stepPosExchange += f(i1.second, i2.second, timeInterval);
		}
	};

	class attractionHandler : public graph::handler
	{
		float scalar(float distance, float averageSize)
		{
			return (distance > averageSize) ?
				2 * (distance - averageSize) / (averageSize * averageSize) : 0;
		}
		glm::vec2 f(visualNode& v1, visualNode& v2, microseconds timeInterval)
		{
			float distance = glm::length(v1.position - v2.position);

			glm::vec2 normal = glm::normalize(v1.position - v2.position);
			glm::vec2 averageSize = (v1.size + v2.size) * 0.5f;
			return glm::vec2
			(
				scalar(distance, averageSize.x) * normal.x * force * (timeInterval / 1ms),
				scalar(distance, averageSize.x) * normal.y * force * (timeInterval / 1ms)
			);
		}
	public:
		float force;
		
		attractionHandler(float fc)
			:force(fc) {}

		virtual ~attractionHandler() override {}

		virtual void update(milliseconds timeInterval) override
		{
			for (auto i1 : nGraph->connections)
				f(nGraph->findNode(i1.first.first), nGraph->findNode(i1.first.second), timeInterval);
		}
	};
	class centripetalHandler : public graph::handler
	{
		glm::vec2 centripet(visualNode& v1, microseconds timeInterval)
		{
			float distance = glm::length(v1.position);
			glm::vec2 normal = glm::normalize(v1.position);
			return glm::vec2
			(
				-glm::sqrt(distance) * normal.x * force * (timeInterval / 1ms),
				-glm::sqrt(distance) * normal.y * force * (timeInterval / 1ms)
			);
		}
	public:
		float force;

		centripetalHandler(float fc)
			:force(fc) {}

		virtual ~centripetalHandler() override {}

		virtual void update(milliseconds timeInterval) override
		{
			for (auto i1 : nGraph->nodes)
				i1.second.stepPosExchange += centripet(i1.second, timeInterval);
		}
	};

	
}

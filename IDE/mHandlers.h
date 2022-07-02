#pragma once
#include "nodeBoard.h"

namespace nechto::ide::handler
{
	using sharedVisualNodePare = std::pair<sharedVisualNode, sharedVisualNode>;
	using nodeInteractionForce = std::function<glm::vec2
	(sharedVisualNodePare, milliseconds)>;

	class repulsionHandler : public nodeBoard::handler
	{
		glm::vec2 repulsion(sharedVisualNodePare vnPare, microseconds timeInterval)
		{
			float distance = glm::length(vnPare.first->position - vnPare.second->position);
			glm::vec2 normal = glm::normalize(vnPare.first->position - vnPare.second->position);
			float averageSize = (vnPare.first->size + vnPare.second->size)/2;
			return glm::vec2
			(
				averageSize / distance * normal.x * force * (timeInterval / 1ms),
				averageSize / distance * normal.y * force * (timeInterval / 1ms)
			);
		}
	public:
		float force;
		
		repulsionHandler(float fc)
			:force(fc) {}

		virtual ~repulsionHandler() override {}

		virtual void update(milliseconds timeInterval) override
		{
			for (auto i1 : nBoard->vNode)
				for (auto i2 : nBoard->vNode)
					if(i1->operator nechto::node::ptr() != i2->operator nechto::node::ptr())
						i1->position += repulsion(sharedVisualNodePare(i1, i2), timeInterval);
		}
	};

	class attractionHandler : public nodeBoard::handler
	{
		glm::vec2 attraction(sharedVisualNodePare vnPare, microseconds timeInterval)
		{
			float distance = glm::length(vnPare.first->position - vnPare.second->position);
			glm::vec2 normal = glm::normalize(vnPare.first->position - vnPare.second->position);
			float averageSize = (vnPare.first->size + vnPare.second->size) / 2;
			return glm::vec2
			(
				-distance / averageSize * normal.x * force * (timeInterval / 1ms),
				-distance / averageSize * normal.y * force * (timeInterval / 1ms)
			);
		}
	public:
		float force;

		attractionHandler(float fc)
			:force(fc) {}

		virtual ~attractionHandler() override {}

		virtual void update(milliseconds timeInterval) override
		{
			for (auto i1 : nBoard->vNode)
				for (auto i2 : i1->connections)
					if (i1->operator nodePtr() != i2.second.lock()->operator nodePtr())
						i1->position += attraction(sharedVisualNodePare(i1, i2.second.lock()), timeInterval);
		}
	};
}

#pragma once
#include "nodeBoard.h"
#include "GLM/glm.hpp"

namespace nechto::ide
{
	class spreadHandler
	{
	public:
		nodePtr nBoardNode;

		float attractionSpeed;
		float repulsionSpeed;
		float centripetSpeed;
		glm::vec2 boardCenter;
		
		spreadHandler(nodePtr nbn, glm::vec2 bCenter, float asp, float rsp, float csp)
			:nBoardNode(nbn), boardCenter(bCenter),
			attractionSpeed(asp), repulsionSpeed(rsp), centripetSpeed(csp) {}

		virtual void update()
		{
			nodeBoard* nBoard = nodeBoard::getByNode(nBoardNode);
			assert(nBoard);
			groupIterator i1(nBoard->vNodeGroup());
			do
			{
				visualNode* vNode = visualNode::getByNode(i1.get());
				if (!vNode)
					continue;
				//для каждой ноды сначала вычесляется смещение, а только потом применяется
				glm::vec2 offset = glm::vec2(0, 0);
				//стремление к центру
				offset += centripet(vNode);
				//отталкивание
				groupIterator i2(nBoard->vNodeGroup());
				do
				{
					visualNode* vNode2 = visualNode::getByNode(i2.get());
					if (vNode2 && (i1.get() != i2.get()))
						offset += repuls(vNode, vNode2->position);
				} while (i2.stepForward());
				//притягивание по соединениям
				connectionIterator i3(i1.get());
				do
				{
					visualConnection* vscon = visualConnection::getByNode(i3.get());
					if (vscon)
					{
						nodePtr v1 = i3.get()->connection[0];
						nodePtr v2 = i3.get()->connection[1];
						//vscon - соединение 2 нод. нужна та, что != i1.get()
						if (v1 == i1.get())
							v1 = v2;
						visualNode* vNode2 = visualNode::getByNode(v1);
						assert(vNode2);
						offset += attract(vNode, vNode2->position);
					}
				} while (i3.stepForward());
				vNode->position += offset;
			} while (i1.stepForward());
		}

		glm::vec2 centripet(visualNode* v1)
		{
			float distance = glm::length(v1->position - boardCenter);
			if (distance < 1)
				return glm::vec2(0, 0);

			glm::vec2 normal = glm::normalize(v1->position - boardCenter);
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
		glm::vec2 attract(visualNode* v1, glm::vec2 pos)
		{
			float distance = glm::length(v1->position - pos);

			glm::vec2 normal = -glm::normalize(v1->position - pos);
			return glm::vec2
			(
				atScalar(distance, v1->size.x) * normal.x * attractionSpeed,
				atScalar(distance, v1->size.y) * normal.y * attractionSpeed
			);
		}
		float repScalar(float distance, float averageSize)
		{
			float scal = (1 / glm::abs(distance - averageSize));
			//if()
			assert(scal != NAN);
			return scal;
		}
		glm::vec2 repuls(visualNode* v1, glm::vec2 pos)
		{
			float distance = glm::length(v1->position - pos);
			if (distance < 0.1)
				return glm::vec2(0, 0);

			glm::vec2 normal = glm::normalize(v1->position - pos);
			return glm::vec2
			(
				repScalar(distance, v1->size.x) * normal.x * repulsionSpeed,
				repScalar(distance, v1->size.y) * normal.y * repulsionSpeed
			);
		}
	};
}

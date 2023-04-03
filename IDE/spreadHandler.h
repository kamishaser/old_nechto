#pragma once
//#include "nodeBoard.h"
//#include "GLM/glm.hpp"

namespace nechto::ide
{
	//class spreadHandler
	//{
	//public:

	//	float attractionSpeed;
	//	float repulsionSpeed;
	//	float centripetSpeed;
	//	glm::vec2 boardCenter;
	//	nodeBoard* nBoard;
	//	
	//	spreadHandler(nodeBoard* nbn, glm::vec2 bCenter, float asp, float rsp, float csp)
	//		:nBoard(nbn), boardCenter(bCenter),
	//		attractionSpeed(asp), repulsionSpeed(rsp), centripetSpeed(csp) {}

	//	virtual void update()
	//	{
	//		assert(nBoard);
	//		groupPointer i1(nBoard->vNodeGroup());
	//		do
	//		{
	//			visualNode* vNode = getEntity<visualNode>(i1.get());
	//			if (!vNode)
	//				continue;
	//			//для каждой ноды сначала вычесляется смещение, а только потом применяется
	//			//стремление к центру
	//			vNode->target += centripet(vNode);
	//			//отталкивание
	//			groupPointer i2(nBoard->vNodeGroup());
	//			do
	//			{
	//				visualNode* vNode2 = getEntity<visualNode>(i2.get());
	//				if (vNode2 && (i1.get() != i2.get()))
	//					vNode->target += repuls(vNode, vNode2);
	//			} while (i2.stepForward());
	//			//притягивание по соединениям
	//			connectionPointer i3(i1.get());
	//			do
	//			{
	//				visualConnection* vscon = getEntity<visualConnection>(i3.get());
	//				if (vscon)
	//				{
	//					nodePtr v1 = i3.get()->connection[0];
	//					nodePtr v2 = i3.get()->connection[1];
	//					//vscon - соединение 2 нод. нужна та, что != i1.get()
	//					if (v1 == i1.get())
	//						v1 = v2;
	//					visualNode* vNode2 = getEntity<visualNode>(v1);
	//					assert(vNode2);
	//					vNode->target += attract(vNode, vNode2);
	//				}
	//			} while (i3.stepForward());
	//		} while (i1.stepForward());
	//	}
	//	glm::vec2 intersect(visualNode* v1, visualNode* v2)
	//	{
	//		return v1->size + v2->size - glm::abs(v1->position - v2->position);
	//	}
	//	
	//	glm::vec2 centripet(visualNode* v1)
	//	{
	//		float distance = glm::length(v1->position - boardCenter);
	//		if (distance < 1)
	//			return glm::vec2(0, 0);

	//		glm::vec2 normal = glm::normalize(v1->position - boardCenter);
	//		return glm::vec2
	//		(
	//			-glm::sqrt(distance) * normal.x * centripetSpeed,
	//			-glm::sqrt(distance) * normal.y * centripetSpeed
	//		);
	//	}
	//	float atScalar(float distance, float averageSize)
	//	{
	//		float scal = (distance > averageSize) ?
	//			(distance - averageSize) / (averageSize) : 0;
	//		return scal;
	//	}
	//	glm::vec2 attract(visualNode* v1, visualNode* v2)
	//	{
	//		glm::vec2 difference = v1->position - v2->position;
	//		glm::vec2 averageSize = (v1->size + v2->size) / 2.f;
	//		float distance = glm::length(difference);

	//		glm::vec2 normal = -glm::normalize(difference);
	//		return glm::vec2
	//		(
	//			atScalar(distance, averageSize.x) * normal.x * attractionSpeed,
	//			atScalar(distance, averageSize.y) * normal.y * attractionSpeed
	//		);
	//	}
	//	float repScalar(float distance, float averageSize)
	//	{
	//		float scal = (1 / glm::abs(distance - averageSize));
	//		//if()
	//		assert(scal != NAN);
	//		return scal;
	//	}
	//	glm::vec2 repuls(visualNode* v1, visualNode* v2)
	//	{
	//		glm::vec2 difference = v1->position - v2->position;
	//		glm::vec2 averageSize = (v1->size + v2->size) / 2.f;
	//		float distance = glm::length(difference);
	//		if (distance < 0.1)
	//			return glm::vec2(0, 0);

	//		glm::vec2 normal = glm::normalize(difference);
	//		return glm::vec2
	//		(
	//			repScalar(distance, averageSize.x) * normal.x * repulsionSpeed,
	//			repScalar(distance, averageSize.y) * normal.y * repulsionSpeed
	//		);
	//	}
	//};
}

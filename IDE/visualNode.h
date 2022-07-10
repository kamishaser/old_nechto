#pragma once
#include "GLM/glm.hpp"
#include "externalConnection.h"
#include "shape.h"

namespace nechto::ide
{
	struct visualNode
	{
		glm::vec2 position{ 0,0 };
		glm::vec2 stepPosExchange{ 0,0 };
		glm::vec2 size;
		shape nShape;
		std::string nodeText = "error";
	};
}

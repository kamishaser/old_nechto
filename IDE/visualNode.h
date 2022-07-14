#pragma once
#include "GLM/glm.hpp"
#include "externalConnection.h"
#include "visual.h"
#include "visualAttribute.h"

namespace nechto::ide
{
	
	struct visualNode
	{
		glm::vec2 position{ 500,500 };
		glm::vec2 stepPosExchange{ 0,0 };
		glm::vec2 size{10,10};
		geometricShape nShape;
		std::wstring nodeText = L"а кириллица тут работает?";
		//externalConnection exCon;
		
		//visualNode(nodePtr v1,)
	};
}

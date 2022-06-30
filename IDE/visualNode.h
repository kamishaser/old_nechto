#pragma once
#include "externalConnection.h"
#include "GLM/glm.hpp"
#include "attribute.h"
#include <memory>


namespace nechto::ide
{
	struct visualVertex
	{
		glm::vec2 position;
		float size;
	};
	struct visualNode : public visualVertex
	{
		
	public:
		externalConnection exCon;

		~visualNode()
		{
			attribute::delAll(exCon.getTag());
		}
	};
}
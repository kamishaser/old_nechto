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
		visualNode(nodePtr v1, comName name)
			:exCon(v1, name){}
		~visualNode()
		{
			attribute::delAll(exCon.getTag());
		}
		void setAttribute(const comName& attributeName)
		{
			attribute::set(exCon.getTag(), attributeName);
		}
		nodePtr getAttribute(const comName& attributeName) const
		{
			return attribute::get(exCon.getTag(), attributeName);
		}
		void deleteAttribure(const comName& attributeName)
		{
			return attribute::delAttribute(exCon.getTag(), attributeName);
		}
		std::set<nodePtr>&& allAtributes()
		{
			return attribute::getAll(exCon.getTag());
		}
	};
}
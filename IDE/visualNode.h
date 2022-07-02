#pragma once
#include "externalConnection.h"
#include "GLM/glm.hpp"
#include "attribute.h"
#include <memory>
#include <random>
#include <map>


namespace nechto::ide
{
	struct visualNode
	{
		externalConnection exCon;
	public:

		std::map<int64_t, std::weak_ptr<visualNode>> connections;
		glm::vec2 position;
		float size;
		
		visualNode(nodePtr v1, glm::vec2 pos, float s)
			:exCon(v1, "IDE.visualNode"_cmn), position(pos), size(s) {}
		~visualNode()
		{
			attribute::delAll(exCon.getTag());
		}

		operator nodePtr() const
		{
			return exCon;
		}
		bool exist() const
		{
			return exCon.exist();
		}
		nodePtr setAttribute(const comName& attributeName)
		{
			return attribute::set(exCon.getTag(), attributeName);
		}
		void setAttribute(nodePtr va)
		{
			assert(typeSubtypeCompare(va, node::Tag, tag::Attribute));
			attribute::set(exCon.getTag(), va);
		}
		nodePtr getAttribute(const comName& attributeName) const
		{
			return attribute::get(exCon.getTag(), attributeName);
		}
		void deleteAttribute(const comName& attributeName)
		{
			return attribute::delAttribute(exCon.getTag(), attributeName);
		}
		std::set<nodePtr> allAtributes() const
		{
			return attribute::getAll(exCon.getTag());
		}
		auto operator <=> (const visualNode& vNode2) const
		{
			return exCon <=> vNode2.exCon;
		}
	};
	using sharedVisualNode = std::shared_ptr<visualNode>;
}
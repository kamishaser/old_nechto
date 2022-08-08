#pragma once
#include "GLM/glm.hpp"
#include "externalConnection.h"
#include "visual.h"
#include "visualAttribute.h"

namespace nechto::ide
{
	
	struct visualNode:public externalConnection
	{
		glm::vec2 position{ 500,500 };
		glm::vec2 stepPosExchange{ 0,0 };
		glm::vec2 size{10,10};

		color shapeColor = color::White;
		color lightColor = color(0, 0, 0);
		geometricShape nShape;

		const static std::wstring vnTypeName;
		std::wstring nodeText = L"а кириллица тут работает?";
	
		visualNode()
			:externalConnection(vnTypeName, newNode(node::ExternalConnection)) {}
		visualNode(nodePtr v1)
			:visualNode()
		{
			NumHubConnect(exCon.load(), v1, 0);
		}
		visualNode(const visualNode& vn)
			:visualNode()
		{
			nodePtr v1 = vn.getConnection(0);
			if(v1.exist())
				NumHubConnect(get(), v1, 0);
		}
		const visualNode& operator= (const visualNode& vn)
		{
			NumHubConnect(get(), (vn.getConnection(0)), 0);
		}
		~visualNode()
		{
			deleteExConNode();
		}
	};
	const std::wstring visualNode::vnTypeName = L"nechtoIde.visualNode";
}

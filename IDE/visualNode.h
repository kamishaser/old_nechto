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
		~visualNode()
		{
			deleteExConNode();
		}
		//проверка: является ли аргуменn внешним подключением visualNode
		static visualNode* getByPtr(nodePtr v1)
		{
			if (v1->getType() != node::ExternalConnection)
				return nullptr;
			auto exCon = v1->getData<externalConnection*>();
			if (exCon == nullptr)
				return nullptr;
			if (exCon->typeName != vnTypeName)
				return nullptr;
			return dynamic_cast<visualNode*>(exCon);
		}
	};
	const std::wstring visualNode::vnTypeName = L"nechtoIde.visualNode";

	
}

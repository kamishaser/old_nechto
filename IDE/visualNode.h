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

		std::wstring nodeText;
	
		visualNode(nodePtr v1  = nullNodePtr)
			:externalConnection(newNode(node::ExternalConnection))
		{
			assert(getByPtr(v1) == nullptr);//vNode нельзя подключать к vNode
			if(v1.exist())
				NumHubConnect(exCon.load(), v1, 0);
		}
		virtual ~visualNode()
		{
			deleteExConNode();
		}
		/*получение указателя на visualNode по объекту. 
		Возвращает nullptr при несоответствии*/
		static visualNode* getByPtr(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalConnection)
				return nullptr;
			auto exCon = v1->getData<externalConnection*>();
			if (exCon == nullptr)
				return nullptr;
			if (exCon->getTypeName() != typeName)
				return nullptr;
			return dynamic_cast<visualNode*>(exCon);
		}
		const static std::wstring typeName;
		virtual const std::wstring& getTypeName() const override
		{
			return typeName;
		}
	};
	const std::wstring visualNode::typeName = L"nechtoIde.visualNode";
}

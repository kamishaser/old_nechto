#pragma once
#include "GLM/glm.hpp"
#include "externalObject.h"
#include "graphOperations.h"
#include "nodeOperationSet.h"
#include "visual.h"

namespace nechto::ide
{
	//к порту 0 подключается отображаемая нода, коли таковая имеется
	//к порту 3 подлкючается nodeBoard
	struct visualNode:public externalObject
	{
		glm::vec2 position{ 500,500 };
		glm::vec2 size{10,10};

		color shapeColor = color::White;
		color lightColor = color(0, 0, 0);
		geometricShape nShape;

		std::wstring nodeText;
	
		visualNode(nodePtr vNodeGroup, nodePtr v1  = nullNodePtr)
			:externalObject(newNode(node::ExternalObject, 1))
			//при удалении ноды, удалится и сей объект !!!только выделять через new!!!
		{
			assert(getByNode(v1) == nullptr);//vNode нельзя подключать к vNode
			assert(vNodeGroup.exist());
			assert(typeCompare(vNodeGroup, node::Group));
			IterIterConnect(group::firstEmptyPort(vNodeGroup), hubIterator(get(), get(), 3));
			if(v1.exist())
				NumHubConnect(exObj, v1, 0);
		}
		virtual ~visualNode()
		{
			std::cout << "vNode deleted" << std::endl;
		}
		/*получение указателя на visualNode по объекту. 
		Возвращает nullptr при несоответствии*/
		static visualNode* getByNode(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalObject)
				return nullptr;
			auto exObj = v1->getData<externalObject*>();
			if (exObj == nullptr)
				return nullptr;
			if (exObj->getTypeName() != typeName)
				return nullptr;
			return dynamic_cast<visualNode*>(exObj);
		}
		const static std::wstring typeName;
		const static staticNodeOperationSet methodSet;
		virtual const std::wstring& getTypeName() const override
		{
			return typeName;
		}
		virtual const operation& getMethod(char number)
		{
			return methodSet.getOperation(number);
		}
	};
	const std::wstring visualNode::typeName = L"nechtoIde.visualNode";
	const staticNodeOperationSet visualNode::methodSet
	{
		namedOperation(L"getNodeText", operation{
				connectionRule(conRule::ExternalObject, conRule::Input, nullptr,
				conRule::Text, conRule::Output),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				text::set(v1, v0->getData<visualNode*>()->nodeText);
				return true;
			}
			}),
		namedOperation(L"setNodeText", operation{
				connectionRule(conRule::ExternalObject, conRule::Output, nullptr,
				conRule::Text, conRule::Input),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				v0->getData<visualNode*>()->nodeText = text::get(v1);
				return true;
			}
			}),
		namedOperation(L"getPosition", operation{
				connectionRule(conRule::ExternalObject, conRule::Input, nullptr,
				conRule::F64Variable, conRule::Output, nullptr,
				conRule::F64Variable, conRule::Output),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				auto* vNode = v0->getData<visualNode*>();
				v1->setData<f64>(vNode->position.x);
				v2->setData<f64>(vNode->position.y);
				return true;
			}
			}),
		namedOperation(L"setPosition", operation{
				connectionRule(conRule::ExternalObject, conRule::Output, nullptr,
				conRule::F64Variable, conRule::Input, nullptr,
				conRule::F64Variable, conRule::Input),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				auto* vNode = v0->getData<visualNode*>();
				vNode->position.x = v1->getData<f64>();
				vNode->position.y = v2->getData<f64>();
				return true;
			}
			}),
		namedOperation(L"move", operation{
				connectionRule(conRule::ExternalObject, conRule::Output, nullptr,
				conRule::F64Variable, conRule::Input, nullptr,
				conRule::F64Variable, conRule::Input),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				auto* vNode = v0->getData<visualNode*>();
				vNode->position.x += v1->getData<f64>();
				vNode->position.y += v2->getData<f64>();
				return true;
			}
			}),
		namedOperation(L"getSize", operation{
				connectionRule(conRule::ExternalObject, conRule::Input, nullptr,
				conRule::F64Variable, conRule::Output, nullptr,
				conRule::F64Variable, conRule::Output),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				auto* vNode = v0->getData<visualNode*>();
				v1->setData<f64>(vNode->size.x);
				v2->setData<f64>(vNode->size.y);
				return true;
			}
			}),
		namedOperation(L"setSize", operation{
				connectionRule(conRule::ExternalObject, conRule::Output, nullptr,
				conRule::F64Variable, conRule::Input, nullptr,
				conRule::F64Variable, conRule::Input),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				auto* vNode = v0->getData<visualNode*>();
				vNode->size.x = v1->getData<f64>();
				vNode->size.y = v2->getData<f64>();
				return true;
			}
			}),
		namedOperation(L"setShape", operation{
				connectionRule(conRule::ExternalObject, conRule::Output, nullptr,
				conRule::I64Variable, conRule::Input),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				auto* vNode = v0->getData<visualNode*>();
				switch (v1->getData<i64>())
				{
				case 0:
					vNode->nShape = vnShape::rectangle();
				case 1:
					vNode->nShape = vnShape::rhombe();
				case 2:
					vNode->nShape = vnShape::octagon();
				case 3:
					vNode->nShape = vnShape::circle();
				default:
					return false;
				}
	
				return true;
			}
			}),
	};
}

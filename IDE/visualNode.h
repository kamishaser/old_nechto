#pragma once
#include "GLM/glm.hpp"
#include "externalObject.h"
#include "nodeOperationSet.h"
#include "visual.h"
#include "rect.h"

namespace nechto::ide
{
	//к порту 0 подключается отображаемая нода, коли таковая имеется
	//к потру 1 подключается группа vNode управляющая этой нодой
	//к порту 3 подлкючается группа всех vNode в nodeBoard

	struct visualNode:public externalObject
	{
		//позиция состоит из текущего и целеввого значений.
		//это нужно для стабильных и плавных перемещений
		rect frame;//текущая рамка
		glm::vec2 target;//цель

		color shapeColor = color::White;
		color lightColor = color(0, 0, 0);
		geometricShape nShape;
		std::wstring nodeText;
	
		visualNode(nodePtr emptyExternalObject, 
			nodePtr vNodeGroup, nodePtr v1  = nullNodePtr)
			:externalObject(emptyExternalObject), frame(glm::vec2{0.f, 0.f}, glm::vec2{1.f, 1.f})
			//при удалении ноды, удалится и сей объект !!!только выделять через new!!!
		{
			assert(getByNode(v1) == nullptr);//vNode нельзя подключать к vNode
			assert(vNodeGroup.exist());
			assert(typeCompare(vNodeGroup, node::Group));
			IterIterConnect(group::firstEmptyPort(vNodeGroup), hubIterator(get(), get(), 3));
			if(v1.exist())
				NumHubConnect(exObj, v1, 0);
		}
		nodePtr getNodeBoard()
		{
			nodePtr temp = get()->connection[3].load();
			if (!temp.exist())
				return nullNodePtr;
			temp = temp->connection[0];
			return temp;
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
			return dynamic_cast<visualNode*>(v1->getData<externalObject*>());
		}
		const static std::wstring typeName;
		const static staticNodeOperationSet methodSet;
		const static connectionRule cRule;
		virtual const std::wstring& getTypeName() const override
		{
			return typeName;
		}
		virtual const operation& getMethod(char number)const override
		{
			return methodSet.getOperation(number);
		}
		virtual const conRule& getConnectionRule()const override
		{
			return cRule;
		}
	};
	const std::wstring visualNode::typeName = L"nechtoIde.visualNode";
	const connectionRule visualNode::cRule = connectionRule{
		conRule::Any, conRule::In_Output, nullptr,
		conRule::Any, conRule::None, nullptr,
		conRule::Any, conRule::None, nullptr,
		conRule::Group, conRule::In_Output,
		[](nodePtr vNodeGroup)->bool {
			nodePtr nBoard;
			if (!typeCompare(nBoard, node::ExternalObject))
				return false;
			if (nBoard->getData<externalObject*>()->getTypeName()
				!= L"nechtoIde.nodeBoard")
				return false;
			return true;
		}
	};
	const staticNodeOperationSet visualNode::methodSet
	{
		/*namedOperation(L"getNodeText", operation{
				connectionRule(
					conRule::ExternalObject, conRule::Input, nullptr,
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
				v1->setData<f64>(vNode->frame.position.x);
				v2->setData<f64>(vNode->frame.position.y);
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
				vNode->frame.position.x = v1->getData<f64>();
				vNode->frame.position.y = v2->getData<f64>();
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
				vNode->frame.position.x += v1->getData<f64>();
				vNode->frame.position.y += v2->getData<f64>();
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
				v1->setData<f64>(vNode->frame.size.x);
				v2->setData<f64>(vNode->frame.size.y);
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
				vNode->frame.size.x = v1->getData<f64>();
				vNode->frame.size.y = v2->getData<f64>();
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
			})*/
	};
}

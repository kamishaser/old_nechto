#pragma once
#include "GLM/glm.hpp"
#include "object.h"
#include "nodeOperationSet.h"
#include "visual.h"
#include "rect.h"

namespace nechto::ide
{
	//� ����� 0 ������������ 
	//	������������ ����, ���� visualNode ������������ ��� �����������,
	//	����������� ��������, ���� visualNode ������������ ��� ������
	//� ����� 1 ������������ visualGroup ����������� ���� �����
	//� ����� 3 ������������ ������ ���� vNode � nodeBoard

	struct visualNode:public externalObject
	{
		//������� ������� �� �������� � ��������� ��������.
		//��� ����� ��� ���������� � ������� �����������
		rect frame;//������� �����

		color shapeColor = col::vNodeG1;
		color lightColor = color(0, 0, 0);
		geometricShape nShape;
		std::u16string nodeText;
	
		visualNode(nodePtr emptyExternalObject, nodePtr v1  = nullNodePtr)
			:externalObject(emptyExternalObject), frame(glm::vec2{0.f, 0.f}, glm::vec2{1.f, 1.f})
			//��� �������� ����, �������� � ��� ������ !!!������ �������� ����� new!!!
		{
			assert(getByNode(v1) == nullptr);//vNode ������ ���������� � vNode
			//IterIterConnect(group::firstEmptyPort(vNodeGroup), hubIterator(get(), get(), 3));
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
			print(u"vNode deleted");
		}
		/*��������� ��������� �� visualNode �� �������. 
		���������� nullptr ��� ��������������*/
		static visualNode* getByNode(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalObject)
				return nullptr;
			return dynamic_cast<visualNode*>(v1->getData<externalObject*>());
		}
		const static std::u16string typeName;
		const static staticNodeOperationSet methodSet;
		const static connectionRule cRule;
		virtual const std::u16string& getTypeName() const override
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
	const std::u16string visualNode::typeName = u"nechtoIde.visualNode";
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
				!= u"nechtoIde.nodeBoard")
				return false;
			return true;
		}
	};
	const staticNodeOperationSet visualNode::methodSet
	{
		/*namedOperation(u"getNodeText", operation{
				connectionRule(
					conRule::ExternalObject, conRule::Input, nullptr,
				conRule::Text, conRule::Output),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				text::set(v1, v0->getData<visualNode*>()->nodeText);
				return true;
			}
			}),
		namedOperation(u"setNodeText", operation{
				connectionRule(conRule::ExternalObject, conRule::Output, nullptr,
				conRule::Text, conRule::Input),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				v0->getData<visualNode*>()->nodeText = text::get(v1);
				return true;
			}
			}),
		namedOperation(u"getPosition", operation{
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
		namedOperation(u"setPosition", operation{
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
		namedOperation(u"move", operation{
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
		namedOperation(u"getSize", operation{
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
		namedOperation(u"setSize", operation{
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
		namedOperation(u"setShape", operation{
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

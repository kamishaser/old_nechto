#pragma once
#include "node.h"
#include "visualNode.h"
#include "group.h"

namespace nechto::ide
{
	
	//к нулевому и первому подключаются ноды
	//к третьему подключается vCon из nodeBoard
	struct visualConnection :public externalObject
	{
		std::u16string fText = u"h";
		std::u16string sText = u"h";

		visualConnection(nodePtr emptyExternalObject, visualNode* vn1, visualNode* vn2)
			:externalObject(emptyExternalObject)
			//при удалении ноды, удалится и сей объект. 
			//!!!только выделять через new!!!
		{
			/*IterIterConnect(group::firstEmptyPort(vConnectionGroup),
				hubIterator(get(), get(), 3));*/
			nodePtr v1 = vn1->exObj;
			nodePtr v2 = vn2->exObj;
			NumHubConnect(exObj, v1, 0);
			NumHubConnect(exObj, v2, 1);
		}
		nodePtr getOtherEnd(nodePtr v1)
		{
			if (get()->connection[0].load() == v1)
				return get()->connection[1];
			else
				return get()->connection[0];
		}
		nodePtr getNodeBoard()
		{
			nodePtr temp = get()->connection[3].load();
			if (!temp.exist())
				return nullNodePtr;
			temp = temp->connection[0];
			return temp;
		}
		virtual ~visualConnection()
		{
			print(u"visualConnectionDeleted");
		}
		/*получение указателя на visualConnection по объекту.
		Возвращает nullptr при несоответствии*/
		static visualConnection* getByNode(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalObject)
				return nullptr;
			return dynamic_cast<visualConnection*>(v1->getData<externalObject*>());
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
	const std::u16string visualConnection::typeName = u"nechtoIde.visualConnection";
	const connectionRule visualConnection::cRule = connectionRule{};
	const staticNodeOperationSet visualConnection::methodSet
	{
		/*namedOperation(u"getOtherEnd", operation{
				connectionRule(conRule::ExternalObject, conRule::Input, nullptr,
				conRule::ExternalObject, conRule::None, [](nodePtr v1)
					{
						return (visualNode::getByNode(v1) != nullptr);
					},
					conRule::AnyPointer_NoTransit, conRule::Output, nullptr),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				auto vConnection = v0->getData<visualConnection*>();
				nodePtr temp = vConnection->getOtherEnd(v1);
				pointer::set(v2, temp);
				return temp.exist();
			}})*/
	};
}
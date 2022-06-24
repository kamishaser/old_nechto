#pragma once
#include "tag.h"
#include "lowLevelGraphOperations.h"

namespace nechto
{
	class externalConnection
	{
		nodePtr exConTag;
		std::string tName; //временное имя. Передаётся тегу при создании
	public:
		externalConnection(nodePtr conNode, std::string& name)
		{
			assert(conNode != nullNodePtr);
			exConTag = newNode(node::Tag, tag::ExternalConnection);
			tag::setData(exConTag, name);
			NumHubConnect(exConTag, conNode, 0);
		}
		explicit externalConnection(std::string& name)
			:tName(name), exConTag(nullNodePtr);
		void setConnaction(nodePtr connaction)
		{
			assert(connaction != nullNodePtr);
			if (exConTag != nullNodePtr)
			{
				exConTag = newNode(node::Tag, tag::ExternalConnection);
				tag::setData(exConTag, tName);
				tName.clear();
				NumHubConnect(exConTag, conNode, 0);
			}
			else
			{
				disconnect(exConTag, exConTag->connection[0]);
				NumHubConnect(exConTag, conNode, 0);
			}
		}
		bool exist() { return exConTag != nullNodePtr; }
		
		operator nodePtr()
		{
			return (exConTag != nullNodePtr)
				? exConTag->connection[0] : nullNodePtr;
		}
		node* operator->()
		{
			return *exConTag->connection[0].load();
		}
		int64_t getI()//возввращает целочисленное значение переменной
		{
			assert(exist());
			nodePtr v1 = exConTag->connection[0];
			assert(v1 != nullNodePtr);
			assert(exConTag->type == node::Variable);
			if ()
		}
	};
}
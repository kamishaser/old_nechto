#pragma once
#include "tag.h"
#include "lowLevelGraphOperations.h"

namespace nechto
{
	class externalConnection
	{
		nodePtr exConTag;
	public:
		externalConnection(const std::string& name, const nodePtr conNode = nullNodePtr)
			:exConTag(newNode(node::Tag, tag::ExternalConnection))
		{
			tag::setData(exConTag, name);
			if(conNode.exist())
				NumHubConnect(exConTag, conNode, 0);
		}
		externalConnection& operator=(const nodePtr conNode)
		{
			disconnect(exConTag, exConTag->connection[0]);
			NumHubConnect(exConTag, conNode, 0);
			return *this;
		}
		bool exist() { return exConTag->hasConnection(0); }
		auto operator <=> (const externalConnection& exCon)
		{
			return tag::getData(exConTag) <=> tag::getData(exCon.exConTag);
		}
		operator const nodePtr()
		{
			return exConTag->connection[0];
		}
		node* operator->()
		{
			return *exConTag->connection[0].load();
		}
		///////////////////////////////////////////////////
		//дополнительные функции для работы с переменными

		operator int64_t()//возввращает значение переменной
		{
			nodePtr v1 = exConTag->connection[0];
			assert(v1.exist());
			assert(exConTag->type == node::Variable);
			if (v1->subtype == baseValueType::Int64)
				return v1->getData<int64_t>();
			return static_cast<int64_t>(v1->getData<double>());
		}
		operator double()//возввращает значение переменной
		{
			nodePtr v1 = exConTag->connection[0];
			assert(v1.exist());
			assert(exConTag->type == node::Variable);
			if (v1->subtype == baseValueType::Double)
				return v1->getData<double>();
			return static_cast<double>(v1->getData<int64_t>());
		}

		externalConnection& operator=(const int64_t value)
		{
			nodePtr v1 = exConTag->connection[0];
			assert(v1.exist());
			assert(exConTag->type == node::Variable);
			if (v1->subtype == baseValueType::Int64)
				v1->setData<int64_t>(value);
			else
				v1->setData<double>(static_cast<double>(value));
		}
		externalConnection& operator=(const double value)
		{
			nodePtr v1 = exConTag->connection[0];
			assert(v1.exist());
			assert(exConTag->type == node::Variable);
			if (v1->subtype == baseValueType::Int64)
				v1->setData<int64_t>(static_cast<int64_t>(value));
			else
				v1->setData<double>(value);
		}
	};
}
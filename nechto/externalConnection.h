#pragma once
#include "tag.h"
#include "nodeOperations.h"
#include "uniqueNodePtr.h"

namespace nechto
{
	class externalConnection
	{
	protected:
		uniqueNodePtr exConTag;
	public:
		externalConnection(const std::string& name, const nodePtr conNode = nullNodePtr)
			:exConTag(uniqueNodePtr::makeUnique(node::Tag, tag::ExternalConnection))
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
		bool exist() const{ return exConTag->hasConnection(0); }
		auto operator <=> (const externalConnection& exCon) const 
		{
			return exConTag <=> exCon.exConTag;
		}
		operator const nodePtr() const
		{
			return exConTag->connection[0];
		}
		node* operator->() const
		{
			return *exConTag->connection[0].load();
		}
		const nodePtr getTag() const
		{
			return exConTag;
		}
		///////////////////////////////////////////////////
		//дополнительные функции для работы с переменными

		//operator int64_t()//возввращает значение переменной
		//{
		//	nodePtr v1 = exConTag->connection[0];
		//	assert(v1.exist());
		//	assert(exConTag->getType() == node::Variable);
		//	if (v1->getSubtype() == baseValueType::Int64)
		//		return v1->getData<int64_t>();
		//	return static_cast<int64_t>(v1->getData<double>());
		//}
		//operator double()//возввращает значение переменной
		//{
		//	nodePtr v1 = exConTag->connection[0];
		//	assert(v1.exist());
		//	assert(exConTag->getType() == node::Variable);
		//	if (v1->getSubtype() == baseValueType::Double)
		//		return v1->getData<double>();
		//	return static_cast<double>(v1->getData<int64_t>());
		//}

		//const externalConnection& operator=(const int64_t value)
		//{
		//	nodePtr v1 = exConTag->connection[0];
		//	assert(v1.exist());
		//	assert(exConTag->getType() == node::Variable);
		//	if (v1->getSubtype() == baseValueType::Int64)
		//		v1->setData<int64_t>(value);
		//	else
		//		v1->setData<double>(static_cast<double>(value));
		//	return *this;
		//}
		//const externalConnection& operator=(const double value)
		//{
		//	nodePtr v1 = exConTag->connection[0];
		//	assert(v1.exist());
		//	assert(exConTag->getType() == node::Variable);
		//	if (v1->getSubtype() == baseValueType::Int64)
		//		v1->setData<int64_t>(static_cast<int64_t>(value));
		//	else
		//		v1->setData<double>(value);
		//	return *this;
		//}
	};
}
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
		externalConnection(const nodePtr conNode, const std::wstring& name)
			:exConTag(node::Tag, tag::ExternalConnection)
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
		nodePtr get()const
		{
			return exConTag->connection[0].load();
		}
		std::wstring getName() const
		{
			return tag::getData(exConTag);
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
		
		externalConnection(const externalConnection& exCon)
			:externalConnection(exCon.getTag(), exCon.getName()) 
		{}
		externalConnection& operator= (const externalConnection& exCon)
		{
			if(exCon.get().exist())
				NumHubConnect(exConTag, exCon.get(), 0);
			tag::setData(exConTag, exCon.getName());
			return *this;
		}
	};
}
#pragma once
#include "namedExCon.h"
#include "connecter.h"
#include "portSearch.h"
#include "group.h"

namespace nechto
{
	struct namedExConGroup :public namedExCon
	{
		namedExConGroup(const std::u16string& n)
			:namedExCon(n)
		{
			
			simplifiedNumNumConnect(node(), creator::createGroup(), 0, 0);
		}
		namedExConGroup(objectNullPtr emptyObject, 
			const std::u16string& n)
			:namedExCon(emptyObject, n)
		{
			simplifiedNumNumConnect(node(), creator::createGroup(), 0, 0);
		}
		virtual ~namedExConGroup()
		{
			if (node().connection(0).exist())
				creator::deleteNode(node().connection(0));
		}
		groupPtr getGroup() const
		{
			return node().connection(0);
		}
		bool contains(nodePtr v1) const
		{
			return groupOperations::contains(getGroup(), v1);
		}
		void addNodeToNearestPort(nodePtr v1) const
		{
			simplifiedIterHubConnect(firstEmptyGroupPort(getGroup()), v1);
		}
		void addNodeToNearestPort(iterator iter) const
		{
			simplifiedIterIterConnect(firstEmptyGroupPort(getGroup()), iter);
		}
		void addNodeToEnd(nodePtr v1) const
		{
			simplifiedIterHubConnect(backGroupPort(getGroup()), v1);
		}
		void addNodeToEnd(iterator iter) const
		{
			simplifiedIterIterConnect(backGroupPort(getGroup()), iter);
		}
		i64 size() const
		{
			return getGroup().getSize();
		}
	};
}
#pragma once
#include "namedExCon.h"
#include "connecter.h"
#include "portSearch.h"
#include "group.h"

namespace nechto
{
	struct namedExConGroup :public namedExCon
	{
		namedExConGroup(const std::wstring& n)
			:namedExCon(n)
		{
			
			NumNumConnect(node(), creator::createGroup(), 0, 0);
		}
		namedExConGroup(nodePtr emptyObject, 
			const std::wstring& n)
			:namedExCon(emptyObject, n)
		{
			NumNumConnect(node(), creator::createGroup(), 0, 0);
		}
		virtual ~namedExConGroup()
		{
			if (node().connection(0).exist())
				creator::deleteNode(node().connection(0));
		}
		nodePtr getGroup() const
		{
			return node().connection(0);
		}
		bool contains(nodePtr v1) const
		{
			return groupOperations::contains(getGroup(), v1);
		}
		void addNodeToNearestPort(nodePtr v1) const
		{
			IterHubConnect(firstEmptyGroupPort(getGroup()), v1);
		}
		void addNodeToNearestPort(iterator iter) const
		{
			IterIterConnect(firstEmptyGroupPort(getGroup()), iter);
		}
		void addNodeToEnd(nodePtr v1) const
		{
			IterHubConnect(backGroupPort(getGroup()), v1);
		}
		void addNodeToEnd(iterator iter) const
		{
			IterIterConnect(backGroupPort(getGroup()), iter);
		}
		i64 size() const
		{
			return groupOperations::numberOfMembers(groupPtr(getGroup()));
		}
	};
}
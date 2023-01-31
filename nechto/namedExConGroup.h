#pragma once
#include "namedExCon.h"
#include "connecter.h"
#include "portSearch.h"
#include "group.h"

namespace nechto
{
	/*struct namedExConGroup :public namedExCon
	{
		namedExConGroup(const std::wstring& n)
			:namedExCon(n)
		{
			
			NumNumConnect(node(), creator::createGroup(), 0, 0);
		}
		namedExConGroup(nodePtr emptyEntity, 
			const std::wstring& n)
			:namedExCon(emptyEntity, n)
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
			PointerHubConnect(firstEmptyGroupPort(getGroup()), v1);
		}
		void addNodeToNearestPort(pointer ptr) const
		{
			PointerPointerConnect(firstEmptyGroupPort(getGroup()), ptr);
		}
		void addNodeToEnd(nodePtr v1) const
		{
			PointerHubConnect(backGroupPort(getGroup()), v1);
		}
		void addNodeToEnd(pointer ptr) const
		{
			PointerPointerConnect(backGroupPort(getGroup()), ptr);
		}
		i64 size() const
		{
			return groupOperations::numberOfMembers(groupPtr(getGroup()));
		}
	};*/
}
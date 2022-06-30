#pragma once
#include "node.h"
#include "tag.h"
#include "nodeOperations.h"

namespace nechto
{
	namespace attribute
	{
		nodePtr get(nodePtr v1, const comName& name)
		{
			nodePtr hubIterator = v1->hubConnection;
			if (!hubIterator.exist())
				return nullNodePtr;
			while (true)
			{
				for (int i = 0; i < 4; i++)
				{
					nodePtr connection = hubIterator->connection[i];
					if (typeSubtypeCompare(connection, node::Tag, tag::Attribute))
					{
						if (comName::isGlobalAndLocal(comName(tag::getData(connection)), name))
							return connection;
					}
				}
				if (!hubIterator->hasHub())
					return nullNodePtr;
				hubIterator = hubIterator->hubConnection;
			}
		}
		void delWithAllParametrs(nodePtr attribute)
		{
			assert(typeSubtypeCompare(attribute, node::Tag, tag::Attribute));
			for (int i = 1; i < 4; ++i)
			{
				nodePtr atVar = attribute->connection[i];
				if (typeCompare(atVar, node::Variable))
				{
					numDisconnect(attribute, i);
					if (!isNodeHasConnections(atVar))
						deleteNode(atVar);
					else
					{
						NumHubConnect(attribute, atVar, i);
					}
				}
			}
			deleteNode(attribute);
		}
		void set(nodePtr v1, const nodePtr attribute)
		{
			assert(typeSubtypeCompare(attribute, node::Tag, tag::Attribute));
			nodePtr temp = get(v1, tag::getData(attribute));
			if (temp.exist())
			{
				delWithAllParametrs(temp);
			}

			NumHubConnect(attribute, v1, 0);
		}

		void delAttribute(nodePtr v1, const comName& name)
		{
			delWithAllParametrs(get(v1, name));
		}
		nodePtr set(nodePtr v1, const comName& name)
		{
			nodePtr attribute = newNode(node::Tag, tag::Attribute);
			tag::setData(attribute, name);
			set(v1, attribute);
			return attribute;
		}

		void delAll(nodePtr v1)
		{
			nodePtr hubIterator = v1->hubConnection;
			while (true)
			{
				for (int i = 0; i < 4; i++)
				{
					nodePtr connection = hubIterator->connection[i];
					if (typeSubtypeCompare(connection, node::Tag, tag::Attribute))
					{
						delWithAllParametrs(connection);
					}
				}
				if (!hubIterator->hasHub())
					return;
				hubIterator = hubIterator->hubConnection;
			}
		}
	}
}

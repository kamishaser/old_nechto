#pragma once
#include "externalConnection.h"
#include "GLM/glm.hpp"
#include <memory>


namespace nechto::ide
{
	class visualNode : public externalConnection
	{
	public:
		glm::vec2 position;
		float size;
		nodePtr getAttribute(const comName& name)
		{
			nodePtr hubIterator = exConTag->hubConnection;
			while (true)
			{
				for (int i = 0; i < 4; i++)
				{
					nodePtr connection = hubIterator->connection[i];
					if (typeSubtypeCompare(connection, node::Tag, tag::SystemAttribute))
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
		
		void setAttribute(const nodePtr attribute)
		{
			assert(typeSubtypeCompare(attribute, node::Tag, tag::SystemAttribute));
			nodePtr temp = getAttribute(tag::getData(attribute));
			if (temp.exist())
			{
				deleteAttribute(temp);
			}

			NumHubConnect(attribute, exConTag, 0);
		}
	protected:
		static void deleteAttribute(nodePtr attribute)
		{
			assert(typeSubtypeCompare(attribute, node::Tag, tag::SystemAttribute));
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
	public:
		void deleteAttribute(const comName& name)
		{
			deleteAttribute(getAttribute(name));
		}
		nodePtr setAttribute(const comName& name)
		{
			nodePtr attribute = newNode(node::Tag, tag::SystemAttribute);
			tag::setData(attribute, name);
			setAttribute(attribute);
			return attribute;
		}
		~visualNode()
		{
			nodePtr hubIterator = exConTag->hubConnection;
			while (true)
			{
				for (int i = 0; i < 4; i++)
				{
					nodePtr connection = hubIterator->connection[i];
					if (typeSubtypeCompare(connection, node::Tag, tag::SystemAttribute))
					{
						deleteAttribute(connection);
					}
				}
				if (!hubIterator->hasHub())
					return;
				hubIterator = hubIterator->hubConnection;
			}
		}
	};
}
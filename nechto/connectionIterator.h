#pragma once
#include "node.h"
#include "pointer.h"

namespace nechto
{
	bool typeSubtypeCompare(nodePtr v1, char type, char subtype);
	class connectionIterator
	{
	public:
		nodePtr baseNode = nullNodePtr;
		nodePtr currentNode = nullNodePtr;
		int64_t position = 0;

		connectionIterator() {};
		connectionIterator(nodePtr base, bool isArrayIterator = false)
		{
			if (!isArrayIterator)
			{
				baseNode = base;
				currentNode = base;
			}
			else
			{
				//assert(typeSubtypeCompare(base, node::Pointer, pointer::Array));
				baseNode = base;
				currentNode = base->connection[0];
			}
		}
		bool operator++()
		{
			++position;
			if ((position & 3ll) != 0)
			{
				
				return true;
			}
			else
			{
				nodePtr nextNode = currentNode->hubConnection.load();
				if (nextNode.exist())
				{
					currentNode = nextNode;
					return true;
				}
				else
				{
					currentNode = baseNode;
					position = 0;
					return false;
				}
			}
		}
		nodePtr get()
		{
			return currentNode->connection[position & 3ll].load();
		}
		int64_t positionNumber()
		{
			return position;
		}
	};
}
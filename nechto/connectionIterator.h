#pragma once
#include "node.h"
#include "pointer.h"

namespace nechto::iterator
{
	struct iteratorData
	{
		nodePtr baseNode = nullNodePtr;
		nodePtr currentNode = nullNodePtr;
		i64 position;
	};
	namespace con
	{
		bool stepForward(iteratorData iterdat)
		{
			++iterdat.position;
			if ((iterdat.position & 3ll) != 0)
				return true;
			else
			{
				nodePtr nextNode = iterdat.currentNode->hubConnection.load();
				iterdat.currentNode = nextNode;
				if (nextNode->getType() != node::Hub)
				{
					iterdat.currentNode = nextNode;
					return true;
				}
				else
				{
					iterdat.currentNode = iterdat.baseNode;
					iterdat.position = 0;
					return false;
				}
			}
		}
		bool stepBack(iteratorData iterdat)
		{
			--iterdat.position
		}
		bool atFirst(iteratorData iterdat);
		bool atLast(iteratorData iterdat);
		bool insertHub(iteratorData iterdat);
		bool eraseHub(iteratorData iterdat);
		
	}
	namespace array
	{
		bool stepForward(iteratorData iterdat);
		bool stepBack(iteratorData iterdat);
		bool atFirst(iteratorData iterdat);
		bool atLast(iteratorData iterdat);
		bool insertHub(iteratorData iterdat);
		bool eraseHub(iteratorData iterdat);

	}
	bool typeSubtypeCompare(nodePtr v1, char type, char subtype);
	class connectionIterator
	{
	public:
		

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
		
		nodePtr get()
		{
			return currentNode->connection[position & 3ll].load();
		}
		i64 positionNumber()
		{
			return position;
		}
	};
}
#pragma once
#include "node.h"

namespace nechto
{
	void deleteNode(nodePtr v1);
	namespace pointer
	{
		enum Type
		{
			Single,		//одиночный
			Iterator,	//итератор 
		};
		//single указатель - связь, нода, указывающая на другую. Сам по себе не является действием.
		//array  указатель - массив NumHub соединений
		bool isCorrect(nodePtr v1)
		{
			assert(v1->getType() == node::Pointer);
			switch (v1->getSubtype())
			{
			case Single:
				return true;
			/*case Array:
				if (!v1->hasConnection(0) || !v1->hasConnection(1))
					return false;
				if (v1->connectionType(0) != node::Hub || v1->connectionType(1) != node::Hub)
					return false;
				if (v1->getData<int64_t>() < 0)
					return false;
				return true;*/

			default:
				break;
			}
			return false;
		}
		//nodePtr getNodePtrFromArray(nodePtr v1, int64_t number)
		//{
		//	assert(v1->getType() == node::Pointer);
		//	assert(v1->getSubtype() == Array);
		//	int64_t size = v1->getData<int64_t>();
		//	if (number < 0 || number >= size)
		//		return nullNodePtr;
		//	//в будующем надо будет доделать отимизацию: ход с двух сторон
		//	nodePtr vIterator = v1->connection[0];
		//	int64_t hubNumber = number >> 2;
		//	int64_t connectionNumber = number - (hubNumber << 2);
		//	for (int64_t i = 0; i < hubNumber; i++)
		//		vIterator = vIterator->hubConnection;
		//	return vIterator->connection[connectionNumber];
		//}
		/*void deletePointer(nodePtr v1)
		{
			assert(v1->getType() == node::Pointer);
			if (v1->getSubtype() == Array)
			{
				nodePtr vIterator = v1->connection[0];
				deleteNode(vIterator);
			}
		}*/		
	}

}
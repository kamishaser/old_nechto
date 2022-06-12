#pragma once
#include "node.h"

namespace nechto
{
	size_t getUniqueID()
	{
		static size_t ID = 0;
		return ++ID;
	}
	
	//std::function<void(nodePtr, size_t)>addTag;
	//std::function<nodePtr(size_t)> GetAddressByID;
	//std::function<>
}
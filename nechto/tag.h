#pragma once
#include "node.h"

namespace nechto
{
	namespace tag
	{
		size_t getUniqueID()
		{
			static size_t ID = 0;
			return ++ID;
		}
		enum Type
		{
			Error,
			Comment, //стандартный текстовый комментарий
			Part,    //часть алгоритма имеющая один вход и один выход
		};
	}
	0) вход
	1) выход

	//std::function<void(nodePtr, size_t)>addTag;
	//std::function<nodePtr(size_t)> GetAddressByID;
	//std::function<>
}
#pragma once
#include "node.h"

namespace nechto
{
	namespace tag
	{
		enum Type
		{
			Error,
			Comment, //стандартный текстовый комментарий
		};
		bool isCorrect(nodePtr tag)
		{
			assert(tag->type == node::Tag);
			
			switch (tag->subtype.load())
			{
			case Comment:
				if (!tag->hasConnection(0))//наличие соединения)
					return false;
				break;
			default:
				return false;
			}
			return true;
		}
	}
	//std::function<void(nodePtr, size_t)>addTag;
	//std::function<nodePtr(size_t)> GetAddressByID;
	//std::function<>
}
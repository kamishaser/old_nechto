#pragma once
#include "node.h"
#include "baseValueTypes.h"

#include <functional>

namespace nechto
{
	struct function
	{
		std::string uniqueName;
		std::function<bool(nodePtr)> isCorrect  = nullptr;
		std::function<void(nodePtr)> Func = nullptr;
	};
}

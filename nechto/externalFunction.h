#pragma once
#include "node.h"
#include "baseValueTypes.h"

#include <functional>

namespace nechto
{
	struct externalFunction
	{
		std::string uniqueName;
		std::function<bool(nodePtr)> isCorrect  = nullptr;
		std::function<void(nodePtr)> Func = nullptr;
	};
}

#pragma once
#include "hubEraser.h"

namespace nechto
{
	class compressor
	{
	public:
		static void compressWithoutMovingIterators();
		static void compressWithMovingIterators();
	};
}
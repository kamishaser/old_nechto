#pragma once
#include "periodLimiter.h"
#include "nodeBoard.h"

namespace nechto::ide
{
	
	class placementHandler
	{
	public:
		
		nodeBoard* nBoard;
		placementHandler(nodeBoard* nbn)
			:nBoard(nbn) {}

		
	};
	
}
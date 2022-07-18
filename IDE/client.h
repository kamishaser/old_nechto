#pragma once

#include "ideDisplay.h"
#include "nodeBoard.h"
#include "mHandlers.h"
#include "autoExpandHandler.h"
#include "userH.h"
#include "vnDataUpdateH.h"


namespace nechto::ide
{
	class client
	{
	public:
		std::shared_ptr<ideDisplay> display;
		std::shared_ptr<nodeBoard> nBoard;

		client(std::shared_ptr<ideDisplay> dp, std::shared_ptr<nodeBoard> nb)
			:display(dp), nBoard(nb) 
		{
			nBoard->addHandler(std::make_shared<handler::repulsionHandler>(2, 10ms));
			nBoard->addHandler(std::make_shared<handler::attractionHandler>(2, 10ms));
			nBoard->addHandler(
				std::make_shared<handler::centripetalHandler>(0.01, glm::vec2(500, 500), 10ms));
			nBoard->addHandler(std::make_shared<handler::autoExpandHandler>());
			nBoard->addHandler(std::make_shared<handler::userH>(display));
			nBoard->addHandler(std::make_shared<handler::vnDataUpdateH>());
		}

		bool update()
		{
			nBoard->update();
			return display->update(nBoard->nGraph);
		}
	};

}

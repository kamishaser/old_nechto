#pragma once

#include "ideDisplay.h"
#include "nodeBoard.h"
#include "mHandlers.h"
#include "autoExpandHandler.h"


namespace nechto::ide
{
	class client
	{
	public:
		std::shared_ptr<ideDisplay> window;
		std::shared_ptr<nodeBoard> nBoard;

		client(std::shared_ptr<ideDisplay> wd, std::shared_ptr<nodeBoard> nb)
			:window(wd), nBoard(nb) 
		{
			nBoard->addHandler(std::make_shared<handler::repulsionHandler>(2, 10ms));
			nBoard->addHandler(std::make_shared<handler::attractionHandler>(2, 10ms));
			nBoard->addHandler(
				std::make_shared<handler::centripetalHandler>(0.003, glm::vec2(500, 500), 10ms));
			nBoard->addHandler(std::make_shared<handler::autoExpandHandler>());
		}

		bool update()
		{
			nBoard->update();
			return window->update(nBoard->nGraph);
		}
	};

}

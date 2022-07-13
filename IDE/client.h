#pragma once

#include "ideWindow.h"
#include "nodeBoard.h"
#include "mHandlers.h"
#include "autoExpandHandler.h"


namespace nechto::ide
{
	class client
	{
	public:
		std::shared_ptr<ideWindow> window;
		std::shared_ptr<nodeBoard> nBoard;

		client(std::shared_ptr<ideWindow> wd, std::shared_ptr<nodeBoard> nb)
			:window(wd), nBoard(nb) 
		{
			nBoard->addHandler(std::make_shared<handler::repulsionHandler>(2.2));
			nBoard->addHandler(std::make_shared<handler::attractionHandler>(4));
			nBoard->addHandler(
				std::make_shared<handler::centripetalHandler>(0.001, glm::vec2(500, 500)));
			nBoard->addHandler(std::make_shared<handler::autoExpandHandler>());
		}

		bool update()
		{
			nBoard->update();
			return window->update(nBoard->nGraph);
		}
	};

}

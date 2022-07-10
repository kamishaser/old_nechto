#include "nechto.h"
#include "commandLine.h"
#include "script.h"

#include "sfmlDisplay.h"
#include "nodeDisplay.h"
#include "visualNode.h"
#include "graph.h"
#include "nodeBoard.h"
#include "attribute.h"
#include "mHandlers.h"
#include "autoExpandHandler.h"
using namespace nechto;
using namespace nechto::ide;

void cracalk(graph& nGraph);
int main()
{
	graph nGraph;
	nodeBoard nBoard;
	cracalk(nGraph);
	nBoard.addHandler(std::make_shared<handler::repulsionHandler>(3.2));
	nBoard.addHandler(std::make_shared<handler::attractionHandler>(3));
	nBoard.addHandler(std::make_shared<handler::centripetalHandler>(0.06));
	nBoard.addHandler(std::make_shared<handler::autoExpandHandler>());

	sf::RenderWindow window(sf::VideoMode(1000, 1000), "nechtoIDE");

	sfmlDisplay nDisplay(window);
	
	periodLimiter plim(20ms, 100ms);
	while (window.isOpen())
	{
		std::this_thread::sleep_for(5ms);
		nBoard.update();
		if (plim.moreThanMin())
		{
			
			window.clear();
			for (auto i : nGraph.connections)
				nDisplay.draw(nGraph.findNode(i.first.first), nGraph.findNode(i.first.second), i.second);
			for (auto i : nGraph.nodes)
				nDisplay.draw(i.second);
			window.display();
			plim.reset();

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}
		}
	}
}


void cracalk(graph& nGraph)
{
	nodePtr sl1 = createVariable(0.0);
	nodePtr sl2 = createVariable(0.0);
	nodePtr result = createVariable(0.0);
	nodePtr summator = createMathOperator(mathOperator::Addition, result, sl1, sl2);
	nodePtr printer = createExternalFunction("consoleOut");
	nodePtr reader1 = createExternalFunction("consoleIn");
	nodePtr reader2 = createExternalFunction("consoleIn");

	


	NumHubConnect(reader1, sl1, 0);
	NumHubConnect(reader2, sl2, 0);
	NumHubConnect(printer, result, 0);

	NumHubConnect(reader1, reader2, 3);
	NumHubConnect(reader2, summator, 3);
	NumHubConnect(summator, printer, 3);
	NumHubConnect(printer, reader1, 3);

	nGraph.addNode(sl1);
	nGraph.addNode(reader1);
	nGraph.connect(visualConnectionID(sl1, reader1));

	nodePtr temp = sl1;
	for (int i = 2; i < 9; ++i)
	{
		nodePtr n = newNode(i, 0);
		HubHubConnect(temp, n);
		temp = n;
	}
}
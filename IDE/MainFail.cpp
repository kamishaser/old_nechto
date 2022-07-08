#include "nechto.h"
#include "commandLine.h"
#include "script.h"


#include "nodeDisplay.h"
#include "locatedNode.h"
#include "nodeBoard.h"
#include "attribute.h"
#include "mHandlers.h"
using namespace nechto;
using namespace nechto::ide;

void cracalk(nodeBoard& nBoard);
int main()
{
	assert(visualNode::Font.loadFromFile("Fonts/arial.ttf"));

	nodeBoard nBoard(glm::vec2(1000, 1000));
	
	cracalk(nBoard);
	nBoard.addHandler(std::make_shared<handler::repulsionHandler>(2));
	nBoard.addHandler(std::make_shared<handler::attractionHandler>(1));
	nBoard.addHandler(std::make_shared<handler::centripetalHandler>(1));
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");

	nodeDisplay nDisplay(nBoard, window);
	
	periodLimiter plim(20ms, 100ms);
	while (window.isOpen())
	{
		std::this_thread::sleep_for(5ms);
		nBoard.update();
		if (plim.moreThanMin())
		{
			
			window.clear();
			nDisplay.update();
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


void cracalk(nodeBoard& nBoard)
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

	nBoard.add(sl1);
	std::cout << std::endl;
	nBoard.add(sl2);
	std::cout << std::endl;
	nBoard.add(result);
	std::cout << std::endl;
	nBoard.add(summator);
	std::cout << std::endl;
	nBoard.add(printer);
	std::cout << std::endl;
	nBoard.add(reader1);
	std::cout << std::endl;
	nBoard.add(reader2);
	std::cout << std::endl;

	nodePtr temp = sl1;
	for (int i = 0; i < 10; ++i)
	{
		nodePtr n = newNode();
		HubHubConnect(temp, n);
		temp = n;
		nBoard.add(n);

	}
}
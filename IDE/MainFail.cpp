#include "nechto.h"

#include "client.h"
#include "nodeBoard.h"
#include "mHandlers.h"
#include "autoExpandHandler.h"
using namespace nechto;
using namespace nechto::ide;

void cracalk(std::shared_ptr<graph> nGraph);
int main()
{
	setlocale(LC_ALL, "Rus");
	std::shared_ptr<graph> nGraph = std::make_shared<graph>();
	cracalk(nGraph);
	client c(std::make_shared<ideDisplay>(), std::make_shared<nodeBoard>(nGraph));
	
	while (c.update())
		std::this_thread::sleep_for(2ms);
	std::cout << "end" << std::endl;
}


void cracalk(std::shared_ptr<graph> nGraph)
{
	nodePtr sl1 = createVariable(0.0);
	nodePtr sl2 = createVariable(0.0);
	nodePtr result = createVariable(0.0);
	nodePtr summator = createMathOperator(mathOperator::Addition, result, sl1, sl2);
	nodePtr printer = createExternalFunction(L"consoleOut");
	nodePtr reader1 = createExternalFunction(L"consoleIn");
	nodePtr reader2 = createExternalFunction(L"consoleIn");

	


	NumHubConnect(reader1, sl1, 0);
	NumHubConnect(reader2, sl2, 0);
	NumHubConnect(printer, result, 0);

	NumHubConnect(reader1, reader2, 3);
	NumHubConnect(reader2, summator, 3);
	NumHubConnect(summator, printer, 3);
	NumHubConnect(printer, reader1, 3);

	nGraph->addNode(sl1);
	nGraph->addNode(reader1);
	nGraph->connect(visualConnectionID(sl1, reader1));

	nodePtr temp = sl1;
	for (int i = 2; i < 9; ++i)
	{
		nodePtr n = newNode(i, 0);
		HubHubConnect(temp, n);
		temp = n;
	}
}
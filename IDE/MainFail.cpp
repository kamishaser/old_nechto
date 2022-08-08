#include "checkAndStep.h"

#include "client.h"
#include "nodeBoard.h"
#include "mHandlers.h"
#include "autoExpandHandler.h"
#include "creationInCode.h"
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
	creationInCode cic;
	nodePtr sl1 = cic.createVariable(1.6);
	nodePtr sl2 = cic.createVariable(2.5);
	nodePtr r1= newNode(node::Pointer, pointer::Reference);
	NumHubConnect(r1, sl1, 0);
	//connectionIterator i1(sl1);
	//connectionIterator i2(sl2);
	//swap(i1, i2);
	nodePtr result = cic.createVariable(0);
	nodePtr summator = cic.createMathOperator(mathOperator::Addition, result, r1, sl2);
	step(summator);
	connectionIterator pi1(summator);
	nodePtr i1 = newNode(node::Pointer, pointer::ConIter);
	do
	{
		pi1.push(i1);
		pi1.pull(i1);
		std::wcout << to_string(pi1.get()) << std::endl;

	} while (pi1.stepForward());
	//nodePtr printer = createExternalFunction(L"consoleOut");
	//nodePtr reader1 = createExternalFunction(L"consoleIn");
	//nodePtr reader2 = createExternalFunction(L"consoleIn");

	


	//NumHubConnect(reader1, sl1, 0);
	//NumHubConnect(reader2, sl2, 0);
	//NumHubConnect(printer, result, 0);

	//NumHubConnect(reader1, reader2, 3);
	//NumHubConnect(reader2, summator, 3);
	//NumHubConnect(summator, printer, 3);
	//NumHubConnect(printer, reader1, 3);

	nGraph->addNode(sl1);
	//nGraph->addNode(reader1);
	//nGraph->connect(visualConnectionID(sl1, reader1));
}
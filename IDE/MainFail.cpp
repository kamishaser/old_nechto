#include "checkAndStep.h"

#include "client.h"
#include "nodeBoard.h"
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

	nodePtr a1 = newNode(node::Group);
	groupIterator ai1(a1);
	IterHubConnect(ai1, sl1);
	//std::wcout << to_string(ai1.currentHub) << to_string(ai1.get()) << std::endl;
	/*ai1.stepForward();
	IterHubConnect(ai1, sl2);
	ai1.stepForward();
	ai1.stepForward();
	IterHubConnect(ai1, summator);
	ai1.insertHub();
	ai1.stepForward();
	ai1.stepForward();
	IterHubConnect(ai1, r1);
	std::wcout << connectionsList(a1) << std::endl;

	group::compress(a1);
	std::wcout << connectionsList(a1) << std::endl;*/
	
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
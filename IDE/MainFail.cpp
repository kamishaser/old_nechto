#include "nechto.h"
#include "commandLine.h"
#include "script.h"

using namespace nechto;
bool variableAtNullConnection(nodePtr v1)
{
	if (!v1->hasConnection(0))//наличие соединения
		return false;
	if (v1->connectionType(0) != node::Variable)
		return false;//проверка типа (только переменные)
	return true;
}

script createCalculator()
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

	

	fileStream scrSave;
	scrSave.sOpen("script.nechto");
	scrSave.save(reader1);
	scrSave.save(reader2);
	scrSave.save(sl1);
	scrSave.save(sl2);
	scrSave.save(result);
	scrSave.save(summator);
	scrSave.save(printer);
	scrSave.close();

	assert(isCorrect(summator));

	return script(reader1);
}


int main()
{
	
	

	addExternalFunction(externalFunction(
		"consoleIn",
		variableAtNullConnection,
		[](nodePtr v1)
		{
			
			double temp;
			if (v1->connectionSubtype(0) == baseValueType::Int64)
			{
				int64_t temp;
				std::cin >> temp;
				v1->connection[0].load()->setData<int64_t>(temp);
				return;
			}
			if (v1->connectionSubtype(0) == baseValueType::Double)
			{
				double temp;
				std::cin >> temp;
				v1->connection[0].load()->setData<double>(temp);
				return;
			}
			return;
		}
	));
	addExternalFunction(externalFunction(
		"consoleOut",
		variableAtNullConnection,
		[](nodePtr v1)
		{
			std::cout << nodeProperties(v1->connection[0].load()) << std::endl;
		}
	));
	
	/*commandLine c;
	while (true)
	{
		try
		{
			std::string temp;
			std::getline(std::cin, temp);
			std::cin.clear();
			std::cout << c(temp) << std::endl;
		}
		catch (std::exception e)
		{
			std::cout << e.what() << std::endl;
		}
	}*/
	createCalculator()();
	fileStream scrSave;
	std::set<nodePtr> temp = scrSave.load("script.nechto");
	assert(!temp.empty());
	script scr(*temp.begin());
	scr();
}
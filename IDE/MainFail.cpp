#include "nechto.h"
#include "commandLine.h"
#include "script.h"

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

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
	sf::RenderWindow sff;

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
	scrSave.saveStart("script.nechto");
	scrSave.save(reader1);
	scrSave.save(reader2);
	scrSave.save(sl1);
	scrSave.save(sl2);
	scrSave.save(result);
	scrSave.save(summator);
	scrSave.save(printer);
	scrSave.end();

	assert(isCorrect(summator));

	return script(reader1);
}


int main()
{
	
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;

	/*addExternalFunction(externalFunction(
		"consoleIn",
		variableAtNullConnection,
		[](nodePtr v1)
		{
			if (v1->connectionSubtype(0) == baseValueType::Int64)
			{
				std::string temp;
				std::cin >> temp;
				try
				{ v1->connection[0].load()->setData<int64_t>(std::stoll(temp)); }
				catch(...)
				{
					v1->connection[0].load()->setData<int64_t>(0);
				}
				
				return;
			}
			if (v1->connectionSubtype(0) == baseValueType::Double)
			{
				std::string temp;
				std::cin >> temp;
				try
				{
					v1->connection[0].load()->setData<double>(std::stod(temp));
				}
				catch (...)
				{
					v1->connection[0].load()->setData<double>(0);
				}
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
	commandLine c;
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
	/*createCalculator()();
	fileStream scrSave;
	std::set<nodePtr> temp = scrSave.load("script.nechto");
	assert(!temp.empty());
	script scr(*temp.begin());
	scr();*/
}
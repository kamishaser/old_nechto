#include "node.h"
#include "lowLevelGraphOperations.h"
#include "baseValueTypes.h"
#include "typeNames.h"
#include "commandLine.h"

#include <iostream>
#include <exception>
#include <algorithm>

using namespace nechto;
int main()
{
	commandLine console;
	std::string line;
	try
	{
		while (true)
		{
			std::getline(std::cin, line);
			std::cin.clear();
			std::cout << console(line) << std::endl;
		}
	}
	catch (std::exception exc)
	{
		std::cout << exc.what() << std::endl;
	}
}

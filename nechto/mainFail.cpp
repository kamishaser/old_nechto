#include "node.h"
#include "lowLevelGraphOperations.h"
#include "additionalCreationFunctions.h"
#include "baseValueTypes.h"
#include "typeNames.h"
#include "commandLine.h"
#include "tag.h"
#include "fileStream.h"

#include <iostream>
#include <exception>
#include <algorithm>

using namespace nechto;

int main()
{
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
	}
}
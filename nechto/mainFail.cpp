#include "node.h"
#include "lowLevelGraphOperations.h"
#include "baseValueTypes.h"

#include <iostream>
#include <exception>

using namespace nechto;
int main()
{
	try
	{
		nodePtr a = newNode();
		nodePtr b = newNode();

	}
	catch (std::exception exc)
	{
		std::cout << exc.what() << std::endl;
	}
}

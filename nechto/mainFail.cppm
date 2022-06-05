import node;
import lowLevelGraphOperations;

import <iostream>;
import <exception>;

using namespace nechto;
int main()
{
	try
	{
		nodePtr a = newVertex();
		addHub(a);
	}
	catch (std::exception exc)
	{
		std::cout << exc.what() << std::endl;
	}
}

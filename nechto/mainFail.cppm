import vertex;
import lowLevelGraphOperations;
import nException;

import <iostream>;
import <exception>;

using namespace nechto;
int main()
{
	try
	{
		vertexPtr a = newVertex();
		addHub(a);
	}
	catch (std::exception exc)
	{
		std::cout << exc.what() << std::endl;
	}
}

#include "nechto.h"
#include "commandLine.h"
int main()
{
	using namespace nechto;
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
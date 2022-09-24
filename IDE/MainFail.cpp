#include "checkAndStep.h"

#include "client.h"
#include "nodeBoard.h"
#include "nodeOperator.h"
#include "connectionRule.h"
using namespace nechto;
using namespace nechto::ide;


void test()
{
	
}
int main()
{
	client c;

	while (c.update())
		std::this_thread::sleep_for(10ms);
	std::cout << "end" << std::endl;
}
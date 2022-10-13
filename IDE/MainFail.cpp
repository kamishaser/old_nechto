#include "checkAndStep.h"

#include "nechtoIde.h"
#include "nodeBoard.h"
#include "nodeOperator.h"
#include "connectionRule.h"
#include "MainFail.h"
using namespace nechto;
using namespace nechto::ide;


int main()
{
	nechtoIDE c;
	while (c.update())
		std::this_thread::sleep_for(10ms);
}
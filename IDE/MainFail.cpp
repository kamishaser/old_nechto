#include "nechtoIde.h"
#include "nodeBoard.h"
#include "MainFail.h"
#include <entity.h>
using namespace nechto;
using namespace nechto::ide;


//int main()
//{
//	nechtoIDE c;
//	while (c.update())
//		std::this_thread::sleep_for(10ms);
//}
// 


int main(int argc, char** argv)
{
	std::vector<std::string> args(argc);
	for (int i = 0; i < argc; ++i)
		args[i] = std::string(argv[i]);
	std::filesystem::path directory(args[0]);
	directory = directory.parent_path();
	std::filesystem::path filepath;
	if (argc > 1)
		filepath = args[1];
	else
		filepath = L"autosave.nechto";
	nechtoIDE c(filepath, directory);
	while (c.update())
		std::this_thread::sleep_for(10ms);
}
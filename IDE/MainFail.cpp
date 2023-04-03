#include "entity.h"
#include "crSet.h"
#include "nechtoIde.h"
#include "pExchange.h"
#include "nechtoIde.h"

using namespace nechto;
using namespace nechto::ide;



int main(int argc, char** argv)
{/*
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "nechtoIDE");
	factory fact;
	nodePtr drablock;
	fact.finishedParts.emplace(L"drablock", drablock);*/

	nechtoIDE n;
	while (n.update());
}
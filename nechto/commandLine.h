#pragma once
#include "textOut.h"

using namespace nechto;

class commandLine
{
	nodePtr v1;
	std::string cutWord(std::string& line);
	std::string commandNew(std::string& line);
	std::string commandDelete(std::string& line);
	std::string commandHubHub(std::string& line);
	std::string commandNumHub(std::string& line);
	std::string commandHubNum(std::string& line);
	std::string commandNumNum(std::string& line);
	std::string commandGo(std::string& line);
	std::string commandThis(std::string& line);
	std::string commandStep(std::string& line);
	std::string commandSet(std::string& line);
	std::string commandSetType(std::string& line);
	std::string commandSetSubtype(std::string& line);
	std::string commandIsCorrect(std::string& line);
	std::string commandConnections(std::string& line);
	std::string commandDisconnect(std::string& line);
public:
	nodePtr stoptr(std::string& line);

	commandLine()
	{
		v1 = newNode();
	}
	std::string operator()(std::string line)
	{
		std::string command;
		command = cutWord(line);
		if (command == "new")			return commandNew(line);
		if (command == "delete")		return commandDelete(line);
		if (command == "hubhub")		return commandHubHub(line);
		if (command == "numhub")		return commandNumHub(line);
		if (command == "hubnum")		return commandHubNum(line);
		if (command == "numnum")		return commandNumNum(line);
		if (command == "connections")	return commandConnections(line);
		if (command == "disconnect")	return commandDisconnect(line);
		if (command == "go")			return commandGo(line);
		if (command == "this")			return commandThis(line);
		//if (command == "step") 			return commandStep(line);
		//if (command == "set") 			return commandSet(line);
		if (command == "setType")		return commandSetType(line);
		if (command == "setSubtype")	return commandSetSubtype(line);
		//if (command == "isCorrect")		return commandIsCorrect(line);
		return "unknown command";

	}
};

std::string commandLine::cutWord(std::string& line)
{
	size_t separator= line.find(' ');
	if (separator == -1)
		return line;
	std::string word = line.substr(0, separator);
	while ((separator < line.size()) && line[separator] == ' ')
		++separator;
	line.erase(line.begin(), line.begin() + separator);
	return word;
}

nodePtr commandLine::stoptr(std::string& line)
{
	nodePtr address;
	try
	{
		address.first = std::stoi(cutWord(line));
		address.second = std::stoi(cutWord(line));
	}
	catch (std::exception)
	{
		return nullNodePtr;
	}
	if (!address.exist())
		return nullNodePtr;
	return address;
}

std::string commandLine::commandNew(std::string& line)
{
	std::string word;
	v1 = newNode();
	if (line.empty())
		return to_string(v1);
	word = cutWord(line);
	ushort type = typeName::find(typeName::nodeT, word);
	if (type == 0)
		return (to_string(v1) + "unknownType: " + word);
	v1->type = type;
	if (line.empty())
		return to_string(v1);
	word = cutWord(line);
	type = typeName::findSubType(type, word);
	if (type == 0)
		return (to_string(v1) + "unknownSubype: " + word);
	v1->subtype = type;
	return to_string(v1);
}

std::string commandLine::commandDelete(std::string& line)
{
	if (line.empty())
	{
		deleteNode(v1);
		return to_string(v1) + " was deleted";
	}
	nodePtr temp = stoptr(line);
	if (temp == nullNodePtr)
		return "error";
	deleteNode(temp);
	return to_string(temp) + " was deleted";
}
std::string commandLine::commandSetType(std::string& line)
{
	if (line.empty())
		return "error";
	ushort type = typeName::find(typeName::nodeT, line);
	if (type == 0)
		return (to_string(v1) + "unknownType: " + line);
	v1->type = type;
	std::string temp = to_string(v1) + ' ' + nodeType(v1) + ' ' + nodeSubtype(v1) + ' ' + nodeData(v1);
}
std::string commandLine::commandSetSubtype(std::string& line)
{
	if (line.empty())
		return "error";
	ushort type = typeName::findSubType(v1->type, line);
	if (type == 0)
		return (to_string(v1) + "unknownSubype: " + line);
	v1->subtype = type;
	std::string temp = to_string(v1) + ' ' + nodeType(v1) + ' ' + nodeSubtype(v1) + ' ' + nodeData(v1);
}
std::string commandLine::commandHubHub(std::string& line)
{
	nodePtr v2 = stoptr(line);
	if (!v2.exist())
		return "error";
	HubHubConnect(v1, v2);
	return to_string(v1);
}
std::string commandLine::commandNumHub(std::string& line)
{
	nodePtr v2 = stoptr(line);
	if (!v2.exist())
		return "error";
	ushort nc1;
	try { nc1 = std::stoi(cutWord(line)); }
	catch (std::exception e)
	{
		return e.what();
	}
	NumHubConnect(v1, v2, nc1);
	return to_string(v1);
}
std::string commandLine::commandHubNum(std::string& line)
{
	nodePtr v2 = stoptr(line);
	if (!v2.exist())
		return "error";
	ushort nc1;
	try { nc1 = std::stoi(cutWord(line)); }
	catch (std::exception e)
	{
		return e.what();
	}
	NumHubConnect(v2, v1, nc1);
	return to_string(v1);
}
std::string commandLine::commandNumNum(std::string& line)
{
	nodePtr v2 = stoptr(line);
	if (!v2.exist())
		return "error";
	ushort nc1, nc2;
	try 
	{ 
		nc1 = std::stoi(cutWord(line));
		nc2 = std::stoi(cutWord(line));
	}
	catch (std::exception e)
	{
		return e.what();
	}
	NumNumConnect(v1, v2, nc1, nc2);
	return to_string(v1);
}
std::string commandLine::commandGo(std::string& line)
{
	nodePtr v2 = stoptr(line);
	if (!v2.exist())
		return "error";
	v1 = v2;
	return to_string(v1) + ' ' + nodeType(v1) + ' ' + nodeSubtype(v1);
}
std::string commandLine::commandThis(std::string& line)
{
	return nodeProperties(v1);
}
std::string commandLine::commandConnections(std::string& line)
{
	std::string temp;
	nodePtr vTemp = v1;
	while (true)
	{
		for (int i = 0; i < 4; i++)
			temp = temp + nodeProperties(vTemp->connection[i].load()) + '\n';
		if (vTemp->hubConnection.load() == nullNodePtr)
			return temp;
		vTemp = v1->hubConnection;
	}
}
std::string commandLine::commandDisconnect(std::string& line)
{
	nodePtr v2 = stoptr(line);
	if (!v2.exist())
		return "error";
	disconnect(v1, v2);
	return to_string(v1);
}
// 
//std::string commandLine::commandStep(std::string& line)
//std::string commandLine::commandSet(std::string& line)
//std::string commandLine::commandIsCorrect(std::string& line)
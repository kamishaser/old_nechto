#pragma once
#include "textOut.h"
#include "fileStream.h"
#include "externalConnection.h"
#include <filesystem>

using namespace nechto;

void loadNode(nodePtr v1)
{
	std::wcout << nodeProperties(v1) << std::endl;
}
class commandLine
{
	externalConnection v1;
	std::wstring cutWord				(std::wstring& line);
	std::wstring commandNew			(std::wstring& line);
	std::wstring commandDelete		(std::wstring& line);
	std::wstring commandHubHub		(std::wstring& line);
	std::wstring commandNumHub		(std::wstring& line);
	std::wstring commandHubNum		(std::wstring& line);
	std::wstring commandGo			(std::wstring& line);
	std::wstring commandThis			(std::wstring& line);
	std::wstring commandStep			(std::wstring& line);
	std::wstring commandSet			(std::wstring& line);
	std::wstring commandSetType		(std::wstring& line);
	std::wstring commandSetSubtype	(std::wstring& line);
	std::wstring commandSetData		(std::wstring& line);
	std::wstring commandIsCorrect	(std::wstring& line);
	std::wstring commandConnections	(std::wstring& line);
	std::wstring commandDisconnect	(std::wstring& line);

	fileStream filehan;
	std::wstring commandSave			(std::wstring& line);
	std::wstring commandIsSaved		(std::wstring& line);
	std::wstring commandEnd		(std::wstring& line);
	std::wstring commandLoad			(std::wstring& line);
public:
	nodePtr stoptr(std::wstring& line);

	commandLine()
		:v1(nullNodePtr, std::wstring(L"commandLine")){}
	std::wstring operator()(std::wstring line)
	{
		std::wstring command;
		command = cutWord(line);
		if (command == L"new")			return commandNew(line);
		if (command == L"delete")		return commandDelete(line);
		if (command == L"hubhub")		return commandHubHub(line);
		if (command == L"numhub")		return commandNumHub(line);
		if (command == L"hubnum")		return commandHubNum(line);
		if (command == L"connections")	return commandConnections(line);
		if (command == L"disconnect")	return commandDisconnect(line);
		if (command == L"go")			return commandGo(line);
		if (command == L"this")			return commandThis(line);
		if (command == L"step") 			return commandStep(line);
		if (command == L"setData") 		return commandSetData(line);
		if (command == L"setType")		return commandSetType(line);
		if (command == L"setSubtype")	return commandSetSubtype(line);
		if (command == L"isCorrect")		return commandIsCorrect(line);

		if (command == L"save")			return commandSave(line);
		if (command == L"isSaved")		return commandIsSaved(line);
		if (command == L"end")			return commandEnd(line);
		if (command == L"load")			return commandLoad(line);

		
		return L"unknown command";

	}
};

std::wstring commandLine::cutWord(std::wstring& line)
{
	size_t separator= line.find(' ');
	if (separator == -1)
		return line;
	std::wstring word = line.substr(0, separator);
	while ((separator < line.size()) && line[separator] == ' ')
		++separator;
	line.erase(line.begin(), line.begin() + separator);
	return word;
}

nodePtr commandLine::stoptr(std::wstring& line)
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

std::wstring commandLine::commandNew(std::wstring& line)
{
	std::wstring word;
	v1 = newNode();
	if (line.empty())
		return to_string(v1);
	word = cutWord(line);
	char type = typeName::find(typeName::nodeT, word);
	if (type == 0)
		return (to_string(v1) + L"unknownType: " + word);
	if (hasSubType(v1))
	{
		char subtype;
		if (line.empty())
			return to_string(v1);
		word = cutWord(line);
		subtype = typeName::findSubtype(type, word);
		if (type == 0)
			return (to_string(v1) + L"unknownSubype: " + word);
		setTypeAndSubtype(v1, type, subtype);
	}
	setTypeAndSubtype(v1, type);
	return to_string(v1);
}

std::wstring commandLine::commandDelete(std::wstring& line)
{
	if (line.empty())
	{
		deleteNode(v1);
		return to_string(v1) + L" was deleted";
	}
	nodePtr temp = stoptr(line);
	if (temp == nullNodePtr)
		return L"error";
	deleteNode(temp);
	return to_string(temp) + L" was deleted";
}
std::wstring commandLine::commandSetType(std::wstring& line)
{
	if (line.empty())
		return L"error";
	char type = typeName::find(typeName::nodeT, line);
	if (type == 0)
		return (to_string(v1) + L"unknownType: " + line);
	setTypeAndSubtype(v1, type);
	return nodeProperties(v1);
}
std::wstring commandLine::commandSetSubtype(std::wstring& line)
{
	if (line.empty())
		return L"error";
	char subtype = typeName::findSubtype(v1->getType(), line);
	if (subtype == 0)
		return (to_string(v1) + L"unknownSubype: " + line);
	setTypeAndSubtype(v1, v1->getType(), subtype);
	return nodeProperties(v1);
}
std::wstring commandLine::commandSetData(std::wstring& line)
{
	if (line.empty())
		return L"error";
	nodePtr ptemp;
	int64_t itemp = 0;
	double  dtemp = 0;
	switch (v1->getType())
	{
	case node::Variable:
		switch (v1->getSubtype())
		{
		case baseValueType::Int64:
			try { itemp = std::stoll(line); }
			catch (...) { return L"error"; }
			v1->setData<int64_t>(itemp);
			return commandThis(line);
		case baseValueType::Double:
			try { dtemp = std::stod(line); }
			catch (...) { return L"error"; }
			v1->setData<double>(dtemp);
			return commandThis(line);
		default:
			return L"error";
		}
	case node::Tag:
		tag::setData(v1, line);
		return nodeProperties(v1);
	case node::ExteralFunction:
		if (!externalFunction::exist(line))
			return L"the function named " + line + L" isn't exist";
		v1->setData(externalFunction::get(line));
		return nodeProperties(v1);

	default:
		break;
	}
	return commandThis(line);
}
std::wstring commandLine::commandHubHub(std::wstring& line)
{
	nodePtr v2 = stoptr(line);
	if (!v2.exist())
		return L"error";
	HubHubConnect(v1, v2);
	return to_string(v1);
}
std::wstring commandLine::commandNumHub(std::wstring& line)
{
	nodePtr v2 = stoptr(line);
	if (!v2.exist())
		return L"error";
	ushort nc1;
	try 
	{ 
		nc1 = std::stoi(cutWord(line)); 
		NumHubConnect(v1, v2, nc1);
	}
	catch (std::exception e)
	{
		return std::wstring(L"error");
	}
	
	return to_string(v1);
}
std::wstring commandLine::commandHubNum(std::wstring& line)
{
	nodePtr v2 = stoptr(line);
	if (!v2.exist())
		return L"error";
	ushort nc1;
	try 
	{ 
		nc1 = std::stoi(cutWord(line)); 
		NumHubConnect(v2, v1, nc1);
	}
	catch (std::exception e)
	{
		return L"error";
	}
	
	return to_string(v1);
}
std::wstring commandLine::commandGo(std::wstring& line)
{
	nodePtr v2 = stoptr(line);
	if (!v2.exist())
		return L"error";
	v1 = v2;
	return to_string(v1) + L' ' + nodeType(v1) + L' ' + nodeSubtype(v1);
}
std::wstring commandLine::commandThis(std::wstring& line)
{
	return nodeProperties(v1);
}
std::wstring commandLine::commandConnections(std::wstring& line)
{
	std::wstring temp;
	nodePtr vTemp = v1;
	while (true)
	{
		for (int i = 0; i < 4; i++)
			temp = temp + nodeProperties(vTemp->connection[i].load()) + L'\n';
		if (!isHubExist(vTemp))
			return temp + L"\nnH";
		vTemp = v1->hubConnection;
		temp += L"\n";
	}
}
std::wstring commandLine::commandDisconnect(std::wstring& line)
{
	nodePtr v2 = stoptr(line);
	if (!v2.exist())
		return L"error";
	disconnect(v1, v2);
	return to_string(v1);
}
// 
std::wstring commandLine::commandStep(std::wstring& line)
{
	if (!v1.get().isCorrect())
		return L"the node isn't correct";
	nodePtr nPos = step(v1);
	if (nPos.exist())
		v1 = nPos;
	else
		return L"end";
	return commandThis(line);
}
//std::wstring commandLine::commandSet(std::wstring& line)
std::wstring commandLine::commandIsCorrect(std::wstring& line)
{
	if (v1.get().isCorrect())
		return L"the node is correct";
	else
		return L"the node isn't correct";
}

std::wstring commandLine::commandSave(std::wstring& line)
{
	std::filesystem::path path(line);
	if (!filehan.isOpen())
		filehan.saveStart(path);
	if (!filehan.isOpen())
		return L"error";
	if (!filehan.isSaved(v1))
		filehan.save(v1);
	else return L"error";
	return L"success";
}
std::wstring commandLine::commandEnd(std::wstring& line)
{
	if (!filehan.isOpen())
		return L"error";
	filehan.end();
	return L"success";
}
std::wstring commandLine::commandLoad(std::wstring& line)
{
	std::filesystem::path path(line);
	if (!filehan.isOpen())
		filehan.loadStart(path);
	if (!filehan.isOpen())
		return L"file " + path.wstring() + L" isn't detected";
	return nodeProperties(filehan.load());
}
std::wstring commandLine::commandIsSaved(std::wstring& line)
{
	if (filehan.isSaved(v1))
		return L"true";
	else
		return L"false";
}
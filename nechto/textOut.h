#pragma once
#include "typeNames.h"
#include "text.h"
#include "connectionIterator.h"
#include "variablePtr.h"

namespace nechto
{
	const std::wstring to_string(nodePtr address)
	{
		return static_cast<std::wstring>(L"[") + 
			std::to_wstring(address.getFirst()) + L' ' + 
			std::to_wstring(address.getSecond()) + L']';
	}
	const std::wstring to_string(hubPosPair hpp)
	{
		return std::wstring(L"{") + to_string(hpp.hub) + std::wstring(L" ") +
			std::to_wstring(hpp.getGlobalPos()) + std::wstring(L" ") +
			((hpp.hub.exist()) ? to_string(hpp.follow()) : std::wstring(L"___")) + std::wstring(L"}");
	}
	std::wstring getUstrObjectData(nonTypedObjectPtr objectNode); //���������� � namedExCon
	const std::wstring nodeData(nodePtr v1)
	{
		if (v1 == nullptr)
			return L"nullptr";
		switch (v1.type())
		{
		case nodeT::Variable:
			if (v1.subtype())
				return std::to_wstring(i64VariablePtr(v1).get());
			else
				return std::to_wstring(f64VariablePtr(v1).get());
		case nodeT::Iterator:
			return to_string(v1.connection(0)) + L"\n" + 
				std::to_wstring(iteratorPtr(v1).getHPPair().getGlobalPos());
		case nodeT::Text:
			if (textPtr(v1).textExist())
				return textPtr(v1);
			else
				return L"nullTextPtr";
		case nodeT::Object:
			return getUstrObjectData(v1);
		default:
			return L"";
		}
	}

	std::wstring nodeProperties(nodePtr v1)
	{
		if (!v1.exist())
			return L"nullNodePtr";
		return std::wstring(to_string(v1) + L' ' + 
			typeName::getTypeAndSubtypeName(v1.type(), v1.subtype()) + L' ' + 
			nodeData(v1));
	}
	std::wstring numConnectionsList(nodePtr v1)
	{
		assert(v1.exist());
		std::wstring temp = nodeProperties(v1) + L"____________________________\n";
		for (int i = 0; i < 4; ++i)
			temp += L"   " + nodeProperties(v1.connection(i)) + L'\n';
		return temp;
	}
	std::wstring connectionsList(nodePtr v1)
	{
		std::wstring temp = nodeProperties(v1) + L'\n';
		if (!v1.exist())
			return temp;
		if (v1.type() == nodeT::Group)
		{
			groupPtr group(v1);
			groupIterator iter(group);
			do
			{
				//print(numConnectionsList(iter.hub));
				temp += numConnectionsList(iter.hub) + L"\n";
			} while (iter.goToNextHub());
			temp += L"\n mainChain:\n";
		}
		portIterator iter(v1);
		do {
			temp += numConnectionsList(iter.hub) + L"\n";
		} while (iter.goToNextHub());
		return temp;
	}
	void pConList(nodePtr v1, std::wstring header)
	{
		std::wcout << header << std::endl;
		if (!v1.exist())
		{
			std::wcout << L"ERROR nullNodePtr" << std::endl;
			return;
		}
		for (int i = 0; i < 4; ++i)
			std::wcout << to_string(v1.connection(i)) << std::endl;
	}
	std::wstring getStringNumberOfNodes(int type)
	{
		return typeName::getTypeName(type) + L": " + std::to_wstring(creator::numberOfTypedNodes[type]) + L"\n";
	}
	std::wstring getStringNumberOfNodesOfAllTypes()
	{
		std::wstring buffer;
		buffer += getStringNumberOfNodes(nodeT::Hub);
		buffer += getStringNumberOfNodes(nodeT::Group);
		buffer += getStringNumberOfNodes(nodeT::Iterator);
		buffer += getStringNumberOfNodes(nodeT::Variable);
		buffer += getStringNumberOfNodes(nodeT::Object);
		buffer += getStringNumberOfNodes(nodeT::Text);
		buffer += getStringNumberOfNodes(nodeT::MathOperator);
		buffer += getStringNumberOfNodes(nodeT::Condition);
		buffer += getStringNumberOfNodes(nodeT::Method);
		return buffer;
	}
}
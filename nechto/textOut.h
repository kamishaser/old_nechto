#pragma once
#include "typeNames.h"
#include "text.h"
#include "nodeTypeProperties.h"
#include "connectionIterator.h"

namespace nechto
{
	const std::wstring to_string(nodePtr address)
	{
		return static_cast<std::wstring>(L"[") + 
			std::to_wstring((int)address.getFirst()) + L' ' + 
			std::to_wstring((int)address.getSecond()) + L']';
	}
	const std::wstring nodeType(nodePtr address)
	{
		return typeName::nodeT[address->getType()];
	}
	const std::wstring nodeSubtype(nodePtr address)
	{
		switch (address->getType())
		{
		case node::Variable:
			return typeName::variableT[address->getSubtype()];
		case node::MathOperator:
			return typeName::mathOperatorShortT[address->getSubtype()];
		case node::Text:
			return typeName::textT[address->getSubtype()];
		case node::Pointer:
			return typeName::pointerT[address->getSubtype()];
		default:
			return L"";
		}
	}
	const std::wstring nodeData(nodePtr v1)
	{
		if (v1 == nullNodePtr)
			return L"error";
		i64 data = v1->getData<i64>();
		void* temptr = &data;
		switch (v1->getType())
		{
		case node::Variable:
			if(v1->getSubtype())
				return std::to_wstring(*static_cast<i64*>(temptr));
			else
				return std::to_wstring(*static_cast<f64*>(temptr));
		case node::Text:
			return text::get(v1);
		case node::Method:
			return method::getMethodName(v1);
		case node::ExternalObject:
			if (v1->getData<externalObject*>())
				return v1->getData<externalObject*>()->getTypeName();
		}
		return L"";
	}

	std::wstring nodeProperties(const nodePtr v1)
	{
		if (!v1.exist())
			return L"nullNodePtr";
		return std::wstring(to_string(v1) + L' ' + nodeType(v1) + L' ' + nodeSubtype(v1) + L' ' + nodeData(v1));
	}
	std::wstring numConnectionsList(const nodePtr v1)
	{
		assert(v1.exist());
		std::wstring temp = nodeProperties(v1) + L"________________________________\n";
		for (int i = 0; i < 4; ++i)
			temp += L"   " + nodeProperties(v1->connection[i].load()) + L'\n';
		return temp;
	}
	std::wstring connectionsList(const nodePtr v1)
	{
		std::wstring temp = nodeProperties(v1) + L'\n';
		if (!v1.exist())
			return temp;
		if (typeCompare(v1, node::Group))
		{
			groupIterator gi(v1);
			do
			{
				std::wcout << numConnectionsList(gi.currentHub) << std::endl;
				temp += numConnectionsList(gi.currentHub) + L"\n";
			} while (gi.GoToNextHub());
			temp += L"\n mainChain:\n";
		}
		connectionIterator i(v1);
		do {
			temp += numConnectionsList(i.currentHub) + L"\n";
		} while (i.GoToNextHub());
		return temp;
	}
}
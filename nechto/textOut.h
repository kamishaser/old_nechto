#pragma once
#include "typeNames.h"
#include "text.h"
#include "connectionIterator.h"

namespace nechto
{
	const std::wstring to_string(nodePtr address)
	{
		return static_cast<std::wstring>(L"[") + std::to_wstring((int)address.getFirst()) + L' ' + std::to_wstring((int)address.getSecond()) + L']';
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
			return typeName::mathOperatorT[address->getSubtype()];
		case node::Text:
			return typeName::tagT[address->getSubtype()];
		default:
			return L"error";
		}
	}
	const std::wstring nodeData(nodePtr v1)
	{
		if (v1 == nullNodePtr)
			return L"error";
		size_t data = v1->getData<size_t>();
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
		case node::ExternalFunction:
			/*if (v1->getData<externalFunction*>() == nullptr)
				return L"nullptr Error";
			else 
				return v1->getData<externalFunction*>()->name;*/
			assert(false);
		}
		return L"error";
	}

	std::wstring nodeProperties(const nodePtr v1)
	{
		if (v1 == nullNodePtr)
			return L"nullNodePtr";
		return std::wstring(to_string(v1) + L' ' + nodeType(v1) + L' ' + nodeSubtype(v1) + L' ' + nodeData(v1));
	}
	std::wstring ConnectionsListText(const nodePtr v1)
	{
		std::wstring temp;
		connectionIterator i(v1);
		do {
			temp += to_string(i.get()) + L'\n';
			if (i.position % 4 == 3)
				temp += '\n';
		} while (i.stepForward());
		return temp;
	}
}
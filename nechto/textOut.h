#pragma once
#include "typeNames.h"

namespace nechto
{
	const std::string to_string(nodePtr address)
	{
		return static_cast<std::string>("[") + std::to_string((int)address.getFirst()) + ' ' + std::to_string((int)address.getSecond()) + ']';
	}
	const std::string nodeType(nodePtr address)
	{
		return typeName::nodeT[address->type];
	}
	const std::string nodeSubtype(nodePtr address)
	{
		switch (address->type)
		{
		case node::Variable:
			return typeName::variableT[address->subtype];
		case node::MathOperator:
			return typeName::mathOperatorT[address->subtype];
		case node::Tag:
			return typeName::tagT[address->subtype];
		default:
			return "error";
		}
	}
	const std::string nodeData(nodePtr v1)
	{
		if (v1 == nullNodePtr)
			return "error";
		size_t data = v1->data.load();
		void* temptr = &data;
		switch (v1->type)
		{
		case node::Variable:
			switch (v1->subtype)
			{
			case baseValueType::Int64:
				return std::to_string(*static_cast<int64_t*>(temptr));
			case baseValueType::Double:
				return std::to_string(*static_cast<double*>(temptr));
			default:
				return "error";
			}
		case node::Tag:
			return tag::getData(v1);
		case node::ExteralFunction:
			if (v1->getData<externalFunction*>() == nullptr)
				return "nullptr Error";
			else 
				return v1->getData<externalFunction*>()->name;
		}
		return "error";
	}

	std::string nodeProperties(const nodePtr v1)
	{
		if (v1 == nullNodePtr)
			return "nullNodePtr";
		return std::string(to_string(v1) + ' ' + nodeType(v1) + ' ' + nodeSubtype(v1) + ' ' + nodeData(v1));
	}
}
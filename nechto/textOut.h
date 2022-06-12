#pragma once
#include "typeNames.h"

namespace nechto
{
	const std::string to_string(nodePtr address)
	{
		return static_cast<std::string>("[") + std::to_string((int)address.first) + ' ' + std::to_string((int)address.second) + ']';
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
			case baseValueType::Float:
				return std::to_string(*static_cast<float*>(temptr));
			case baseValueType::Double:
				return std::to_string(*static_cast<double*>(temptr));
			case baseValueType::NodePtr:
				return to_string(*static_cast<nodePtr*>(temptr));
			default:
				return "error";
			}
		}
		return "error";
	}

	std::string nodeProperties(const nodePtr v1)
	{
		if (v1 == nullNodePtr)
			return "nullNodePtr";
		return to_string(v1) + ' ' + nodeType(v1) + ' ' + nodeSubtype(v1) + ' ' + nodeData(v1);
	}
}
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
		size_t data = v1->data.load();
		void* temptr = &data;
		switch (v1->type)
		{
		case node::Variable:
			switch (v1->subtype)
			{
			case baseValueType::Bool:
				return std::to_string(*static_cast<bool*>(temptr));
			case baseValueType::Int8:
				return std::to_string(*static_cast<signed char*>(temptr));
			case baseValueType::uInt8:
				return std::to_string(*static_cast<char*>(temptr));
			case baseValueType::Int16:
				return std::to_string(*static_cast<short*>(temptr));
			case baseValueType::uInt16:
				return std::to_string(*static_cast<ushort*>(temptr));
			case baseValueType::Int32:
				return std::to_string(*static_cast<long*>(temptr));
			case baseValueType::uInt32:
				return std::to_string(*static_cast<unsigned long*>(temptr));
			case baseValueType::Int64:
				return std::to_string(*static_cast<long long*>(temptr));
			case baseValueType::uInt64:
				return std::to_string(*static_cast<size_t*>(temptr));
			case baseValueType::Float:
				return std::to_string(*static_cast<float*>(temptr));
			case baseValueType::Double:
				return std::to_string(*static_cast<double*>(temptr));
			default:
				throw;
			}
		}
		return std::string(" ");
	}

	std::string nodeProperties(const nodePtr v1)
	{
		if (v1 == nullNodePtr)
			return "nullNodePtr";
		return to_string(v1) + ' ' + nodeType(v1) + ' ' + nodeSubtype(v1) + ' ' + nodeData(v1);
	}
}
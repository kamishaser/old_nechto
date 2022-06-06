#pragma once
#include "node.h"

namespace nechto
{
	void typeCast(void* from, void* to, int fromType, int toType);
	bool isTypeCastOperatorCorrect(nodePtr typeCastOperator);
	namespace baseValueType
	{
		enum Type
		{
			Bool,

			Int8,
			uInt8,
			Int16,
			uInt16,
			Int32,
			uInt32,
			Int64,
			uInt64,

			Float,
			Double,
		};
	}

	void typeCast(nodePtr typeCastOperator)
	{
		assert(isTypeCastOperatorCorrect(typeCastOperator));
		size_t buffer;
		ushort varType[2];
		buffer = typeCastOperator->connection[1].load()->data.load();
		varType[0] = typeCastOperator->connection[0].load()->subtype;
		varType[1] = typeCastOperator->connection[1].load()->subtype;
		if (varType[0] != varType[1])
		{
			typeCast(&buffer, &buffer, varType[1], varType[0]);
		}
		else
			typeCastOperator->connection[0].load()->data.store(buffer);
	}

	bool isTypeCastOperatorCorrect(nodePtr typeCastOperator)
	{
		assert(typeCastOperator->type.load() == node::TypeCastOperator);
		if (!typeCastOperator->hasConnection(0)
			|| !typeCastOperator->hasConnection(1))
			return false;
		if (typeCastOperator->connection[0].load()->type != node::Variable
			|| typeCastOperator->connection[1].load()->type != node::Variable)
			return false;
		return true;
	}

	void typeCast(void* from, void* to, int fromType, int toType)
	{
		
		switch (toType)
		{
		case baseValueType::Bool:
			switch (fromType)
			{
			case baseValueType::Bool:
				*static_cast<bool*>(to) = static_cast<bool>(*static_cast<bool*>(from));
				return;
			case baseValueType::Int8:
				*static_cast<bool*>(to) = static_cast<bool>(*static_cast<signed char*>(from));
				return;
			case baseValueType::uInt8:
				*static_cast<bool*>(to) = static_cast<bool>(*static_cast<char*>(from));
				return;
			case baseValueType::Int16:
				*static_cast<bool*>(to) = static_cast<bool>(*static_cast<short*>(from));
				return;
			case baseValueType::uInt16:
				*static_cast<bool*>(to) = static_cast<bool>(*static_cast<ushort*>(from));
				return;
			case baseValueType::Int32:
				*static_cast<bool*>(to) = static_cast<bool>(*static_cast<long*>(from));
				return;
			case baseValueType::uInt32:
				*static_cast<bool*>(to) = static_cast<bool>(*static_cast<unsigned long*>(from));
				return;
			case baseValueType::Int64:
				*static_cast<bool*>(to) = static_cast<bool>(*static_cast<long long*>(from));
				return;
			case baseValueType::uInt64:
				*static_cast<bool*>(to) = static_cast<bool>(*static_cast<size_t*>(from));
				return;
			case baseValueType::Float:
				*static_cast<bool*>(to) = static_cast<bool>(*static_cast<float*>(from));
				return;
			case baseValueType::Double:
				*static_cast<bool*>(to) = static_cast<bool>(*static_cast<double*>(from));
				return;
			}
		case baseValueType::Int8:
			switch (fromType)
			{
			case baseValueType::Bool:
				*static_cast<signed char*>(to) = static_cast<signed char>(*static_cast<bool*>(from));
				return;
			case baseValueType::Int8:
				*static_cast<signed char*>(to) = static_cast<signed char>(*static_cast<signed char*>(from));
				return;
			case baseValueType::uInt8:
				*static_cast<signed char*>(to) = static_cast<signed char>(*static_cast<char*>(from));
				return;
			case baseValueType::Int16:
				*static_cast<signed char*>(to) = static_cast<signed char>(*static_cast<short*>(from));
				return;
			case baseValueType::uInt16:
				*static_cast<signed char*>(to) = static_cast<signed char>(*static_cast<ushort*>(from));
				return;
			case baseValueType::Int32:
				*static_cast<signed char*>(to) = static_cast<signed char>(*static_cast<long*>(from));
				return;
			case baseValueType::uInt32:
				*static_cast<signed char*>(to) = static_cast<signed char>(*static_cast<unsigned long*>(from));
				return;
			case baseValueType::Int64:
				*static_cast<signed char*>(to) = static_cast<signed char>(*static_cast<long long*>(from));
				return;
			case baseValueType::uInt64:
				*static_cast<signed char*>(to) = static_cast<signed char>(*static_cast<size_t*>(from));
				return;
			case baseValueType::Float:
				*static_cast<signed char*>(to) = static_cast<signed char>(*static_cast<float*>(from));
				return;
			case baseValueType::Double:
				*static_cast<signed char*>(to) = static_cast<signed char>(*static_cast<double*>(from));
				return;
			}
		case baseValueType::uInt8:
			switch (fromType)
			{
			case baseValueType::Bool:
				*static_cast<char*>(to) = static_cast<char>(*static_cast<bool*>(from));
				return;
			case baseValueType::Int8:
				*static_cast<char*>(to) = static_cast<char>(*static_cast<signed char*>(from));
				return;
			case baseValueType::uInt8:
				*static_cast<char*>(to) = static_cast<char>(*static_cast<char*>(from));
				return;
			case baseValueType::Int16:
				*static_cast<char*>(to) = static_cast<char>(*static_cast<short*>(from));
				return;
			case baseValueType::uInt16:
				*static_cast<char*>(to) = static_cast<char>(*static_cast<ushort*>(from));
				return;
			case baseValueType::Int32:
				*static_cast<char*>(to) = static_cast<char>(*static_cast<long*>(from));
				return;
			case baseValueType::uInt32:
				*static_cast<char*>(to) = static_cast<char>(*static_cast<unsigned long*>(from));
				return;
			case baseValueType::Int64:
				*static_cast<char*>(to) = static_cast<char>(*static_cast<long long*>(from));
				return;
			case baseValueType::uInt64:
				*static_cast<char*>(to) = static_cast<char>(*static_cast<size_t*>(from));
				return;
			case baseValueType::Float:
				*static_cast<char*>(to) = static_cast<char>(*static_cast<float*>(from));
				return;
			case baseValueType::Double:
				*static_cast<char*>(to) = static_cast<char>(*static_cast<double*>(from));
				return;
			}
		case baseValueType::Int16:
			switch (fromType)
			{
			case baseValueType::Bool:
				*static_cast<short*>(to) = static_cast<short>(*static_cast<bool*>(from));
				return;
			case baseValueType::Int8:
				*static_cast<short*>(to) = static_cast<short>(*static_cast<signed char*>(from));
				return;
			case baseValueType::uInt8:
				*static_cast<short*>(to) = static_cast<short>(*static_cast<char*>(from));
				return;
			case baseValueType::Int16:
				*static_cast<short*>(to) = static_cast<short>(*static_cast<short*>(from));
				return;
			case baseValueType::uInt16:
				*static_cast<short*>(to) = static_cast<short>(*static_cast<ushort*>(from));
				return;
			case baseValueType::Int32:
				*static_cast<short*>(to) = static_cast<short>(*static_cast<long*>(from));
				return;
			case baseValueType::uInt32:
				*static_cast<short*>(to) = static_cast<short>(*static_cast<unsigned long*>(from));
				return;
			case baseValueType::Int64:
				*static_cast<short*>(to) = static_cast<short>(*static_cast<long long*>(from));
				return;
			case baseValueType::uInt64:
				*static_cast<short*>(to) = static_cast<short>(*static_cast<size_t*>(from));
				return;
			case baseValueType::Float:
				*static_cast<short*>(to) = static_cast<short>(*static_cast<float*>(from));
				return;
			case baseValueType::Double:
				*static_cast<short*>(to) = static_cast<short>(*static_cast<double*>(from));
				return;
			}
		case baseValueType::uInt16:
			switch (fromType)
			{
			case baseValueType::Bool:
				*static_cast<ushort*>(to) = static_cast<ushort>(*static_cast<bool*>(from));
				return;
			case baseValueType::Int8:
				*static_cast<ushort*>(to) = static_cast<ushort>(*static_cast<signed char*>(from));
				return;
			case baseValueType::uInt8:
				*static_cast<ushort*>(to) = static_cast<ushort>(*static_cast<char*>(from));
				return;
			case baseValueType::Int16:
				*static_cast<ushort*>(to) = static_cast<ushort>(*static_cast<short*>(from));
				return;
			case baseValueType::uInt16:
				*static_cast<ushort*>(to) = static_cast<ushort>(*static_cast<ushort*>(from));
				return;
			case baseValueType::Int32:
				*static_cast<ushort*>(to) = static_cast<ushort>(*static_cast<long*>(from));
				return;
			case baseValueType::uInt32:
				*static_cast<ushort*>(to) = static_cast<ushort>(*static_cast<unsigned long*>(from));
				return;
			case baseValueType::Int64:
				*static_cast<ushort*>(to) = static_cast<ushort>(*static_cast<long long*>(from));
				return;
			case baseValueType::uInt64:
				*static_cast<ushort*>(to) = static_cast<ushort>(*static_cast<size_t*>(from));
				return;
			case baseValueType::Float:
				*static_cast<ushort*>(to) = static_cast<ushort>(*static_cast<float*>(from));
				return;
			case baseValueType::Double:
				*static_cast<ushort*>(to) = static_cast<ushort>(*static_cast<double*>(from));
				return;
			}
		case baseValueType::Int32:
			switch (fromType)
			{
			case baseValueType::Bool:
				*static_cast<long*>(to) = static_cast<long>(*static_cast<bool*>(from));
				return;
			case baseValueType::Int8:
				*static_cast<long*>(to) = static_cast<long>(*static_cast<signed char*>(from));
				return;
			case baseValueType::uInt8:
				*static_cast<long*>(to) = static_cast<long>(*static_cast<char*>(from));
				return;
			case baseValueType::Int16:
				*static_cast<long*>(to) = static_cast<long>(*static_cast<short*>(from));
				return;
			case baseValueType::uInt16:
				*static_cast<long*>(to) = static_cast<long>(*static_cast<ushort*>(from));
				return;
			case baseValueType::Int32:
				*static_cast<long*>(to) = static_cast<long>(*static_cast<long*>(from));
				return;
			case baseValueType::uInt32:
				*static_cast<long*>(to) = static_cast<long>(*static_cast<unsigned long*>(from));
				return;
			case baseValueType::Int64:
				*static_cast<long*>(to) = static_cast<long>(*static_cast<long long*>(from));
				return;
			case baseValueType::uInt64:
				*static_cast<long*>(to) = static_cast<long>(*static_cast<size_t*>(from));
				return;
			case baseValueType::Float:
				*static_cast<long*>(to) = static_cast<long>(*static_cast<float*>(from));
				return;
			case baseValueType::Double:
				*static_cast<long*>(to) = static_cast<long>(*static_cast<double*>(from));
				return;
			}
		case baseValueType::uInt32:
			switch (fromType)
			{
			case baseValueType::Bool:
				*static_cast<unsigned long*>(to) = static_cast<unsigned long>(*static_cast<bool*>(from));
				return;
			case baseValueType::Int8:
				*static_cast<unsigned long*>(to) = static_cast<unsigned long>(*static_cast<signed char*>(from));
				return;
			case baseValueType::uInt8:
				*static_cast<unsigned long*>(to) = static_cast<unsigned long>(*static_cast<char*>(from));
				return;
			case baseValueType::Int16:
				*static_cast<unsigned long*>(to) = static_cast<unsigned long>(*static_cast<short*>(from));
				return;
			case baseValueType::uInt16:
				*static_cast<unsigned long*>(to) = static_cast<unsigned long>(*static_cast<ushort*>(from));
				return;
			case baseValueType::Int32:
				*static_cast<unsigned long*>(to) = static_cast<unsigned long>(*static_cast<long*>(from));
				return;
			case baseValueType::uInt32:
				*static_cast<unsigned long*>(to) = static_cast<unsigned long>(*static_cast<unsigned long*>(from));
				return;
			case baseValueType::Int64:
				*static_cast<unsigned long*>(to) = static_cast<unsigned long>(*static_cast<long long*>(from));
				return;
			case baseValueType::uInt64:
				*static_cast<unsigned long*>(to) = static_cast<unsigned long>(*static_cast<size_t*>(from));
				return;
			case baseValueType::Float:
				*static_cast<unsigned long*>(to) = static_cast<unsigned long>(*static_cast<float*>(from));
				return;
			case baseValueType::Double:
				*static_cast<unsigned long*>(to) = static_cast<unsigned long>(*static_cast<double*>(from));
				return;
			}
		case baseValueType::Int64:
			switch (fromType)
			{
			case baseValueType::Bool:
				*static_cast<long long*>(to) = static_cast<long long>(*static_cast<bool*>(from));
				return;
			case baseValueType::Int8:
				*static_cast<long long*>(to) = static_cast<long long>(*static_cast<signed char*>(from));
				return;
			case baseValueType::uInt8:
				*static_cast<long long*>(to) = static_cast<long long>(*static_cast<char*>(from));
				return;
			case baseValueType::Int16:
				*static_cast<long long*>(to) = static_cast<long long>(*static_cast<short*>(from));
				return;
			case baseValueType::uInt16:
				*static_cast<long long*>(to) = static_cast<long long>(*static_cast<ushort*>(from));
				return;
			case baseValueType::Int32:
				*static_cast<long long*>(to) = static_cast<long long>(*static_cast<long*>(from));
				return;
			case baseValueType::uInt32:
				*static_cast<long long*>(to) = static_cast<long long>(*static_cast<unsigned long*>(from));
				return;
			case baseValueType::Int64:
				*static_cast<long long*>(to) = static_cast<long long>(*static_cast<long long*>(from));
				return;
			case baseValueType::uInt64:
				*static_cast<long long*>(to) = static_cast<long long>(*static_cast<size_t*>(from));
				return;
			case baseValueType::Float:
				*static_cast<long long*>(to) = static_cast<long long>(*static_cast<float*>(from));
				return;
			case baseValueType::Double:
				*static_cast<long long*>(to) = static_cast<long long>(*static_cast<double*>(from));
				return;
			}
		case baseValueType::uInt64:
			switch (fromType)
			{
			case baseValueType::Bool:
				*static_cast<size_t*>(to) = static_cast<size_t>(*static_cast<bool*>(from));
				return;
			case baseValueType::Int8:
				*static_cast<size_t*>(to) = static_cast<size_t>(*static_cast<signed char*>(from));
				return;
			case baseValueType::uInt8:
				*static_cast<size_t*>(to) = static_cast<size_t>(*static_cast<char*>(from));
				return;
			case baseValueType::Int16:
				*static_cast<size_t*>(to) = static_cast<size_t>(*static_cast<short*>(from));
				return;
			case baseValueType::uInt16:
				*static_cast<size_t*>(to) = static_cast<size_t>(*static_cast<ushort*>(from));
				return;
			case baseValueType::Int32:
				*static_cast<size_t*>(to) = static_cast<size_t>(*static_cast<long*>(from));
				return;
			case baseValueType::uInt32:
				*static_cast<size_t*>(to) = static_cast<size_t>(*static_cast<unsigned long*>(from));
				return;
			case baseValueType::Int64:
				*static_cast<size_t*>(to) = static_cast<size_t>(*static_cast<long long*>(from));
				return;
			case baseValueType::uInt64:
				*static_cast<size_t*>(to) = static_cast<size_t>(*static_cast<size_t*>(from));
				return;
			case baseValueType::Float:
				*static_cast<size_t*>(to) = static_cast<size_t>(*static_cast<float*>(from));
				return;
			case baseValueType::Double:
				*static_cast<size_t*>(to) = static_cast<size_t>(*static_cast<double*>(from));
				return;
			}
		case baseValueType::Float:
			switch (fromType)
			{
			case baseValueType::Bool:
				*static_cast<float*>(to) = static_cast<float>(*static_cast<bool*>(from));
				return;
			case baseValueType::Int8:
				*static_cast<float*>(to) = static_cast<float>(*static_cast<signed char*>(from));
				return;
			case baseValueType::uInt8:
				*static_cast<float*>(to) = static_cast<float>(*static_cast<char*>(from));
				return;
			case baseValueType::Int16:
				*static_cast<float*>(to) = static_cast<float>(*static_cast<short*>(from));
				return;
			case baseValueType::uInt16:
				*static_cast<float*>(to) = static_cast<float>(*static_cast<ushort*>(from));
				return;
			case baseValueType::Int32:
				*static_cast<float*>(to) = static_cast<float>(*static_cast<long*>(from));
				return;
			case baseValueType::uInt32:
				*static_cast<float*>(to) = static_cast<float>(*static_cast<unsigned long*>(from));
				return;
			case baseValueType::Int64:
				*static_cast<float*>(to) = static_cast<float>(*static_cast<long long*>(from));
				return;
			case baseValueType::uInt64:
				*static_cast<float*>(to) = static_cast<float>(*static_cast<size_t*>(from));
				return;
			case baseValueType::Float:
				*static_cast<float*>(to) = static_cast<float>(*static_cast<float*>(from));
				return;
			case baseValueType::Double:
				*static_cast<float*>(to) = static_cast<float>(*static_cast<double*>(from));
				return;
			}
		case baseValueType::Double:
			switch (fromType)
			{
			case baseValueType::Bool:
				*static_cast<double*>(to) = static_cast<double>(*static_cast<bool*>(from));
				return;
			case baseValueType::Int8:
				*static_cast<double*>(to) = static_cast<double>(*static_cast<signed char*>(from));
				return;
			case baseValueType::uInt8:
				*static_cast<double*>(to) = static_cast<double>(*static_cast<char*>(from));
				return;
			case baseValueType::Int16:
				*static_cast<double*>(to) = static_cast<double>(*static_cast<short*>(from));
				return;
			case baseValueType::uInt16:
				*static_cast<double*>(to) = static_cast<double>(*static_cast<ushort*>(from));
				return;
			case baseValueType::Int32:
				*static_cast<double*>(to) = static_cast<double>(*static_cast<long*>(from));
				return;
			case baseValueType::uInt32:
				*static_cast<double*>(to) = static_cast<double>(*static_cast<unsigned long*>(from));
				return;
			case baseValueType::Int64:
				*static_cast<double*>(to) = static_cast<double>(*static_cast<long long*>(from));
				return;
			case baseValueType::uInt64:
				*static_cast<double*>(to) = static_cast<double>(*static_cast<size_t*>(from));
				return;
			case baseValueType::Float:
				*static_cast<double*>(to) = static_cast<double>(*static_cast<float*>(from));
				return;
			case baseValueType::Double:
				*static_cast<double*>(to) = static_cast<double>(*static_cast<double*>(from));
				return;
			}
		}
	}
}

/*
switch (toType)
		{
		case baseValueType::Bool:

		case baseValueType::Int8:

		case baseValueType::uInt8:

		case baseValueType::Int16:

		case baseValueType::uInt16:

		case baseValueType::Int32:

		case baseValueType::uInt32:

		case baseValueType::Int64:

		case baseValueType::uInt64:

		case baseValueType::Float:

		case baseValueType::Double:

		}
		*/
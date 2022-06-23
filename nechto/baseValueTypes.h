#pragma once
#include "node.h"

namespace nechto
{
	//сей файл аписывает используемые типы переменных, преобразование типов
	//а также работу оператора преобразование типа
	void typeCast(void* from, void* to, int fromType, int toType);
	bool isTypeCastOperatorCorrect(nodePtr typeCastOperator);
	namespace baseValueType
	{
		enum Type
		{
			Error,
			Int64,
			Double,
			
		};
	}

	void typeCast(nodePtr typeCastOperator)
	{
		assert(isTypeCastOperatorCorrect(typeCastOperator));
		size_t buffer = 0;
		ushort varType[2]{0,0};
		buffer = typeCastOperator->connection[1].load()->data.load();
		varType[0] = typeCastOperator->connection[0].load()->subtype;
		varType[1] = typeCastOperator->connection[1].load()->subtype;
		if (varType[0] != varType[1])
		{
			typeCast(&buffer, &buffer, varType[1], varType[0]);
		}
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

	bool boolCast(nodePtr Variable)
	{
		assert(Variable->type = node::Variable);
		size_t buffer = Variable->data;
		void* temprt = &buffer;
		switch (Variable->subtype)
		{
		case baseValueType::Int64:
			return static_cast<bool>(*static_cast<int64_t*>(temprt));
		case baseValueType::Double:
			return static_cast<bool>(*static_cast<double*>(temprt));
		default:
			throw;
		}
	}

	void typeCast(void* from, void* to, int fromType, int toType)
	{
		//функция преобразования
		switch (toType)
		{
		case baseValueType::Int64:
			switch (fromType)
			{
			case baseValueType::Int64:
				*static_cast<int64_t*>(to) = static_cast<int64_t>(*static_cast<int64_t*>(from));
				return;
			case baseValueType::Double:
				*static_cast<int64_t*>(to) = static_cast<int64_t>(*static_cast<double*>(from));
				return;
			}
		case baseValueType::Double:
			switch (fromType)
			{
			case baseValueType::Int64:
				*static_cast<double*>(to) = static_cast<double>(*static_cast<int64_t*>(from));
				return;
			case baseValueType::Double:
				*static_cast<double*>(to) = static_cast<double>(*static_cast<double*>(from));
				return;
			}
		default: throw;
		}
		throw;
	}
	
}
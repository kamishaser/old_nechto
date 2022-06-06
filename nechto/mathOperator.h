#pragma once
#include "node.h"
#include "baseValueTypes.h"

namespace nechto
{
	namespace mathOperator
	{
		enum Types
		{
			Assigment,		// =		
			UnaryMinus,		// -

			Addition,		// +
			Subtraction,	// -

			Multiplication, // *
			Division,		// /

			Equal,			// ==
			NotEqual,		// !=

			Less,			// <
			Greater,		// >
			LessOrEqual,	// <=
			GreaterOrEqual,	// >=

			LogicNegation,	// !
			LogicAnd,		// &&
			LogicOr,		// ||
			LogicExclusive,	// ^

			Increment,		// ++
			Decrement		// --
		};
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool isCorrect(nodePtr operatorNode);//проверка корректности узла оператора

		template <class TCon>
		size_t unaryMinus(void* firstValue)
		{
			size_t temp;
			void* temptr = &temp;
			*static_cast<TCon*>(temptr) = -(*static_cast<TCon*>(firstValue));
			return temp;
		}
		template <class TCon>
		size_t logicNegation(void* firstValue)
		{
			size_t temp;
			void* temptr = &temp;
			*static_cast<bool*>(temptr) = !(*static_cast<TCon*>(firstValue));
			return temp;
		}

		template <class TCon>
		size_t addition(void* firstValue, void* secondValue)
		{
			size_t temp;
			void* temptr = &temp;
			*static_cast<TCon*>(temptr) =
				(*static_cast<TCon*>(firstValue)) + (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t substraction(void* firstValue, void* secondValue)
		{
			size_t temp;
			void* temptr = &temp;
			*static_cast<TCon*>(temptr) =
				(*static_cast<TCon*>(firstValue)) - (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t multiplication(void* firstValue, void* secondValue)
		{
			size_t temp;
			void* temptr = &temp;
			*static_cast<TCon*>(temptr) =
				(*static_cast<TCon*>(firstValue)) * (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t division(void* firstValue, void* secondValue)
		{
			size_t temp;
			void* temptr = &temp;
			*static_cast<TCon*>(temptr) =
				(*static_cast<TCon*>(firstValue)) / (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t equal(void* firstValue, void* secondValue)
		{
			size_t temp;
			void* temptr = &temp;
			*static_cast<bool*>(temptr) =
				(*static_cast<TCon*>(firstValue)) == (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t notEqual(void* firstValue, void* secondValue)
		{
			size_t temp;
			void* temptr = &temp;
			*static_cast<bool*>(temptr) =
				(*static_cast<TCon*>(firstValue)) != (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t less(void* firstValue, void* secondValue)
		{
			size_t temp;
			void* temptr = &temp;
			*static_cast<bool*>(temptr) =
				(*static_cast<TCon*>(firstValue)) < (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t greater(void* firstValue, void* secondValue)
		{
			size_t temp;
			void* temptr = &temp;
			*static_cast<bool*>(temptr) =
				(*static_cast<TCon*>(firstValue)) > (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t lessOrEqual(void* firstValue, void* secondValue)
		{
			size_t temp;
			void* temptr = &temp;
			*static_cast<bool*>(temptr) =
				(*static_cast<TCon*>(firstValue)) <= (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t greaterOrEqual(void* firstValue, void* secondValue)
		{
			size_t temp;
			void* temptr = &temp;
			*static_cast<bool*>(temptr) =
				(*static_cast<TCon*>(firstValue)) >= (*static_cast<TCon*>(secondValue));
			return temp;
		}

		template <class TCon>
		size_t logicAnd(void* firstValue, void* secondValue)
		{
			size_t temp;
			void* temptr = &temp;
			*static_cast<bool*>(temptr) =
				(*static_cast<TCon*>(firstValue)) && (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t logicOr(void* firstValue, void* secondValue)
		{
			size_t temp;
			void* temptr = &temp;
			*static_cast<bool*>(temptr) =
				(*static_cast<TCon*>(firstValue)) || (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t logicExclusive(void* firstValue, void* secondValue)
		{
			size_t temp;
			void* temptr = &temp;
			*static_cast<bool*>(temptr) =
				(*static_cast<TCon*>(firstValue)) ^ (*static_cast<TCon*>(secondValue));
			return temp;
		}

		template <class TCon>
		size_t increment(void* firstValue)
		{
			size_t temp;
			void* temptr = &temp;
			*static_cast<TCon*>(temptr) = *static_cast<TCon*>(firstValue) + 1;
			return temp;
		}
		template <class TCon>
		size_t decrement(void* firstValue)
		{
			size_t temp;
			void* temptr = &temp;
			*static_cast<TCon*>(temptr) = *static_cast<TCon*>(firstValue) - 1;
			return temp;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void mathOperation(nodePtr operatorNode)
		{
			assert(isCorrect(operatorNode));


			ushort resultType = operatorNode->connection[0].load()->subtype;
			ushort comVarType;
			size_t temp;
			size_t firstValue;
			size_t secondValue;

			switch (operatorNode->subtype)
			{
			case  Assigment:
				operatorNode->connection[0].load()->data.
					store(operatorNode->connection[1].load()->data.load());

				break;

			case  UnaryMinus:
				firstValue = operatorNode->connection[1].load()->data.load();
				switch (resultType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Int8:
					temp = unaryMinus<signed char>(&firstValue);
					break;
				case baseValueType::Int16:
					temp = unaryMinus<short>(&firstValue);
					break;
				case baseValueType::Int32:
					temp = unaryMinus<long>(&firstValue);
					break;
				case baseValueType::Int64:
					temp = unaryMinus<long long>(&firstValue);
					break;
				case baseValueType::Float:
					temp = unaryMinus<float>(&firstValue);
					break;
				case baseValueType::Double:
					temp = unaryMinus<double>(&firstValue);
					break;
				default:
					throw;
				}
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			case  Addition:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				switch (resultType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Int8:
					temp = addition<signed char>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt8:
					temp = addition<char>(&firstValue, &secondValue);
					break;
				case baseValueType::Int16:
					temp = addition<short>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt16:
					temp = addition<ushort>(&firstValue, &secondValue);
					break;
				case baseValueType::Int32:
					temp = addition<long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt32:
					temp = addition<unsigned long>(&firstValue, &secondValue);
					break;
				case baseValueType::Int64:
					temp = addition<long long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt64:
					temp = addition<size_t>(&firstValue, &secondValue);
					break;
				case baseValueType::Float:
					temp = addition<float>(&firstValue, &secondValue);
					break;
				case baseValueType::Double:
					temp = addition<double>(&firstValue, &secondValue);
					break;
				default:
					throw;
				}
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  Subtraction:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				switch (resultType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Int8:
					temp = substraction<signed char>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt8:
					temp = substraction<char>(&firstValue, &secondValue);
					break;
				case baseValueType::Int16:
					temp = substraction<short>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt16:
					temp = substraction<ushort>(&firstValue, &secondValue);
					break;
				case baseValueType::Int32:
					temp = substraction<long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt32:
					temp = substraction<unsigned long>(&firstValue, &secondValue);
					break;
				case baseValueType::Int64:
					temp = substraction<long long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt64:
					temp = substraction<size_t>(&firstValue, &secondValue);
					break;
				case baseValueType::Float:
					temp = substraction<float>(&firstValue, &secondValue);
					break;
				case baseValueType::Double:
					temp = substraction<double>(&firstValue, &secondValue);
						break;
				default:
					throw;
				}
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  Multiplication:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				switch (resultType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Int8:
					temp = multiplication<signed char>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt8:
					temp = multiplication<char>(&firstValue, &secondValue);
					break;
				case baseValueType::Int16:
					temp = multiplication<short>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt16:
					temp = multiplication<ushort>(&firstValue, &secondValue);
					break;
				case baseValueType::Int32:
					temp = multiplication<long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt32:
					temp = multiplication<unsigned long>(&firstValue, &secondValue);
					break;
				case baseValueType::Int64:
					temp = multiplication<long long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt64:
					temp = multiplication<size_t>(&firstValue, &secondValue);
					break;
				case baseValueType::Float:
					temp = multiplication<float>(&firstValue, &secondValue);
					break;
				case baseValueType::Double:
					temp = multiplication<double>(&firstValue, &secondValue);
						break;
				default:
					throw;
				}
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  Division:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				switch (resultType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Int8:
					temp = division<signed char>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt8:
					temp = division<char>(&firstValue, &secondValue);
					break;
				case baseValueType::Int16:
					temp = division<short>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt16:
					temp = division<ushort>(&firstValue, &secondValue);
					break;
				case baseValueType::Int32:
					temp = division<long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt32:
					temp = division<unsigned long>(&firstValue, &secondValue);
					break;
				case baseValueType::Int64:
					temp = division<long long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt64:
					temp = division<size_t>(&firstValue, &secondValue);
					break;
				case baseValueType::Float:
					temp = division<float>(&firstValue, &secondValue);
					break;
				case baseValueType::Double:
					temp = division<double>(&firstValue, &secondValue);
						break;
				default:
					throw;
				}
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			case  Equal:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Bool:
					temp = equal<bool>(&firstValue, &secondValue);
					break;
				case baseValueType::Int8:
					temp = equal<signed char>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt8:
					temp = equal<char>(&firstValue, &secondValue);
					break;
				case baseValueType::Int16:
					temp = equal<short>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt16:
					temp = equal<ushort>(&firstValue, &secondValue);
					break;
				case baseValueType::Int32:
					temp = equal<long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt32:
					temp = equal<unsigned long>(&firstValue, &secondValue);
					break;
				case baseValueType::Int64:
					temp = equal<long long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt64:
					temp = equal<size_t>(&firstValue, &secondValue);
					break;
				case baseValueType::Float:
					temp = equal<float>(&firstValue, &secondValue);
					break;
				case baseValueType::Double:
					temp = equal<double>(&firstValue, &secondValue);
					break;
				default:
					throw;
				}
				if (resultType != baseValueType::Bool)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Bool, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  NotEqual:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Bool:
					temp = notEqual<bool>(&firstValue, &secondValue);
					break;
				case baseValueType::Int8:
					temp = notEqual<signed char>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt8:
					temp = notEqual<char>(&firstValue, &secondValue);
					break;
				case baseValueType::Int16:
					temp = notEqual<short>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt16:
					temp = notEqual<ushort>(&firstValue, &secondValue);
					break;
				case baseValueType::Int32:
					temp = notEqual<long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt32:
					temp = notEqual<unsigned long>(&firstValue, &secondValue);
					break;
				case baseValueType::Int64:
					temp = notEqual<long long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt64:
					temp = notEqual<size_t>(&firstValue, &secondValue);
					break;
				case baseValueType::Float:
					temp = notEqual<float>(&firstValue, &secondValue);
					break;
				case baseValueType::Double:
					temp = notEqual<double>(&firstValue, &secondValue);
					break;
				default:
					throw;
				}
				if (resultType != baseValueType::Bool)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Bool, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  Less:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Bool:
					temp = less<bool>(&firstValue, &secondValue);
					break;
				case baseValueType::Int8:
					temp = less<signed char>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt8:
					temp = less<char>(&firstValue, &secondValue);
					break;
				case baseValueType::Int16:
					temp = less<short>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt16:
					temp = less<ushort>(&firstValue, &secondValue);
					break;
				case baseValueType::Int32:
					temp = less<long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt32:
					temp = less<unsigned long>(&firstValue, &secondValue);
					break;
				case baseValueType::Int64:
					temp = less<long long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt64:
					temp = less<size_t>(&firstValue, &secondValue);
					break;
				case baseValueType::Float:
					temp = less<float>(&firstValue, &secondValue);
					break;
				case baseValueType::Double:
					temp = less<double>(&firstValue, &secondValue);
						break;
				default:
					throw;
				}
				if (resultType != baseValueType::Bool)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Bool, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  Greater:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Bool:
					temp = greater<bool>(&firstValue, &secondValue);
					break;
				case baseValueType::Int8:
					temp = greater<signed char>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt8:
					temp = greater<char>(&firstValue, &secondValue);
					break;
				case baseValueType::Int16:
					temp = greater<short>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt16:
					temp = greater<ushort>(&firstValue, &secondValue);
					break;
				case baseValueType::Int32:
					temp = greater<long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt32:
					temp = greater<unsigned long>(&firstValue, &secondValue);
					break;
				case baseValueType::Int64:
					temp = greater<long long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt64:
					temp = greater<size_t>(&firstValue, &secondValue);
					break;
				case baseValueType::Float:
					temp = greater<float>(&firstValue, &secondValue);
					break;
				case baseValueType::Double:
					temp = greater<double>(&firstValue, &secondValue);
						break;
				default:
					throw;
				}
				if (resultType != baseValueType::Bool)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Bool, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  LessOrEqual:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Bool:
					temp = lessOrEqual<bool>(&firstValue, &secondValue);
					break;
				case baseValueType::Int8:
					temp = lessOrEqual<signed char>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt8:
					temp = lessOrEqual<char>(&firstValue, &secondValue);
					break;
				case baseValueType::Int16:
					temp = lessOrEqual<short>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt16:
					temp = lessOrEqual<ushort>(&firstValue, &secondValue);
					break;
				case baseValueType::Int32:
					temp = lessOrEqual<long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt32:
					temp = lessOrEqual<unsigned long>(&firstValue, &secondValue);
					break;
				case baseValueType::Int64:
					temp = lessOrEqual<long long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt64:
					temp = lessOrEqual<size_t>(&firstValue, &secondValue);
					break;
				case baseValueType::Float:
					temp = lessOrEqual<float>(&firstValue, &secondValue);
					break;
				case baseValueType::Double:
					temp = lessOrEqual<double>(&firstValue, &secondValue);
						break;
				default:
					throw;
				}
				if (resultType != baseValueType::Bool)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Bool, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  GreaterOrEqual:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Bool:
					temp = greaterOrEqual<bool>(&firstValue, &secondValue);
					break;
				case baseValueType::Int8:
					temp = greaterOrEqual<signed char>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt8:
					temp = greaterOrEqual<char>(&firstValue, &secondValue);
					break;
				case baseValueType::Int16:
					temp = greaterOrEqual<short>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt16:
					temp = greaterOrEqual<ushort>(&firstValue, &secondValue);
					break;
				case baseValueType::Int32:
					temp = greaterOrEqual<long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt32:
					temp = greaterOrEqual<unsigned long>(&firstValue, &secondValue);
					break;
				case baseValueType::Int64:
					temp = greaterOrEqual<long long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt64:
					temp = greaterOrEqual<size_t>(&firstValue, &secondValue);
					break;
				case baseValueType::Float:
					temp = greaterOrEqual<float>(&firstValue, &secondValue);
					break;
				case baseValueType::Double:
					temp = greaterOrEqual<double>(&firstValue, &secondValue);
						break;
				default:
					throw;
				}
				if (resultType != baseValueType::Bool)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Bool, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			case  LogicNegation:
				firstValue = operatorNode->connection[1].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Bool:
					temp = logicNegation<bool>(&firstValue);
					break;
				case baseValueType::Int8:
					temp = logicNegation<signed char>(&firstValue);
					break;
				case baseValueType::uInt8:
					temp = logicNegation<char>(&firstValue);
					break;
				case baseValueType::Int16:
					temp = logicNegation<short>(&firstValue);
					break;
				case baseValueType::uInt16:
					temp = logicNegation<ushort>(&firstValue);
					break;
				case baseValueType::Int32:
					temp = logicNegation<long>(&firstValue);
					break;
				case baseValueType::uInt32:
					temp = logicNegation<unsigned long>(&firstValue);
					break;
				case baseValueType::Int64:
					temp = logicNegation<long long>(&firstValue);
					break;
				case baseValueType::uInt64:
					temp = logicNegation<size_t>(&firstValue);
					break;
				default:
					throw;
				}
				if (resultType != baseValueType::Bool)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Bool, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  LogicAnd:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Bool:
					temp = logicAnd<bool>(&firstValue, &secondValue);
					break;
				case baseValueType::Int8:
					temp = logicAnd<signed char>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt8:
					temp = logicAnd<char>(&firstValue, &secondValue);
					break;
				case baseValueType::Int16:
					temp = logicAnd<short>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt16:
					temp = logicAnd<ushort>(&firstValue, &secondValue);
					break;
				case baseValueType::Int32:
					temp = logicAnd<long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt32:
					temp = logicAnd<unsigned long>(&firstValue, &secondValue);
					break;
				case baseValueType::Int64:
					temp = logicAnd<long long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt64:
					temp = logicAnd<size_t>(&firstValue, &secondValue);
					break;
				default:
					throw;
				}
				if (resultType != baseValueType::Bool)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Bool, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  LogicOr:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Bool:
					temp = logicOr<bool>(&firstValue, &secondValue);
					break;
				case baseValueType::Int8:
					temp = logicOr<signed char>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt8:
					temp = logicOr<char>(&firstValue, &secondValue);
					break;
				case baseValueType::Int16:
					temp = logicOr<short>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt16:
					temp = logicOr<ushort>(&firstValue, &secondValue);
					break;
				case baseValueType::Int32:
					temp = logicOr<long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt32:
					temp = logicOr<unsigned long>(&firstValue, &secondValue);
					break;
				case baseValueType::Int64:
					temp = logicOr<long long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt64:
					temp = logicOr<size_t>(&firstValue, &secondValue);
					break;
				default:
					throw;
				}
				if (resultType != baseValueType::Bool)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Bool, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  LogicExclusive:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Bool:
					temp = logicExclusive<bool>(&firstValue, &secondValue);
					break;
				case baseValueType::Int8:
					temp = logicExclusive<signed char>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt8:
					temp = logicExclusive<char>(&firstValue, &secondValue);
					break;
				case baseValueType::Int16:
					temp = logicExclusive<short>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt16:
					temp = logicExclusive<ushort>(&firstValue, &secondValue);
					break;
				case baseValueType::Int32:
					temp = logicExclusive<long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt32:
					temp = logicExclusive<unsigned long>(&firstValue, &secondValue);
					break;
				case baseValueType::Int64:
					temp = logicExclusive<long long>(&firstValue, &secondValue);
					break;
				case baseValueType::uInt64:
					temp = logicExclusive<size_t>(&firstValue, &secondValue);
					break;
				default:
					throw;
				}
				if (resultType != baseValueType::Bool)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Bool, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			case  Increment:
				firstValue = operatorNode->connection[1].load()->data.load();
				switch (resultType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Int8:
					temp = increment<signed char>(&firstValue);
					break;
				case baseValueType::uInt8:
					temp = increment<char>(&firstValue);
					break;
				case baseValueType::Int16:
					temp = increment<short>(&firstValue);
					break;
				case baseValueType::uInt16:
					temp = increment<ushort>(&firstValue);
					break;
				case baseValueType::Int32:
					temp = increment<long>(&firstValue);
					break;
				case baseValueType::uInt32:
					temp = increment<unsigned long>(&firstValue);
					break;
				case baseValueType::Int64:
					temp = increment<long long>(&firstValue);
					break;
				case baseValueType::uInt64:
					temp = increment<size_t>(&firstValue);
					break;
				default:
					throw;
				}
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  Decrement:
				firstValue = operatorNode->connection[1].load()->data.load();
				switch (resultType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Int8:
					temp = decrement<signed char>(&firstValue);
					break;
				case baseValueType::uInt8:
					temp = decrement<char>(&firstValue);
					break;
				case baseValueType::Int16:
					temp = decrement<short>(&firstValue);
					break;
				case baseValueType::uInt16:
					temp = decrement<ushort>(&firstValue);
					break;
				case baseValueType::Int32:
					temp = decrement<long>(&firstValue);
					break;
				case baseValueType::uInt32:
					temp = decrement<unsigned long>(&firstValue);
					break;
				case baseValueType::Int64:
					temp = decrement<long long>(&firstValue);
					break;
				case baseValueType::uInt64:
					temp = decrement<size_t>(&firstValue);
					break;
				default:
					throw;
				}
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			default:
				throw;
			}
		}

		bool isCorrect(nodePtr operatorNode)
		{
			assert(operatorNode->type == node::MathOperator);
			switch (operatorNode->subtype)
			{
			case  Assigment://////////////////////////////////////////////
				if (!operatorNode->hasConnection(0)//наличие соединения
					|| !operatorNode->hasConnection(1))
					return false;
				if (operatorNode->connectionType(0) != node::Variable
					|| operatorNode->connectionType(1) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(0)
					!= operatorNode->connectionSubtype(1))
					return false;//соответствие типов данных
				break;

			case  UnaryMinus://////////////////////////////////////////////
				if (!operatorNode->hasConnection(0)//наличие соединения
					|| !operatorNode->hasConnection(1))
					return false;
				if (operatorNode->connectionType(0) != node::Variable
					|| operatorNode->connectionType(1) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(0)
					!= operatorNode->connectionSubtype(1))
					return false;//соответствие типов данных
				if (operatorNode->connectionSubtype(0) == baseValueType::Bool)
					return false;//только signed
				if (operatorNode->connectionSubtype(0) == baseValueType::uInt8)
					return false;//только signed
				if (operatorNode->connectionSubtype(0) == baseValueType::uInt16)
					return false;//только signed
				if (operatorNode->connectionSubtype(0) == baseValueType::uInt32)
					return false;//только signed
				if (operatorNode->connectionSubtype(0) == baseValueType::uInt64)
					return false;//только signed
				break;
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			case  Addition://////////////////////////////////////////////
				if (!operatorNode->hasConnection(0)//наличие соединения
					|| !operatorNode->hasConnection(1)
					|| !operatorNode->hasConnection(2))
					return false;
				if (operatorNode->connectionType(0) != node::Variable
					|| operatorNode->connectionType(1) != node::Variable
					|| operatorNode->connectionType(2) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(0)
					!= operatorNode->connectionSubtype(1)
					|| operatorNode->connectionSubtype(0)
					!= operatorNode->connectionSubtype(2))
					return false;//соответствие типов данных
				if (operatorNode->connectionSubtype(0) == baseValueType::Bool)
					return false;
				break;

			case  Subtraction://////////////////////////////////////////////
				if (!operatorNode->hasConnection(0)//наличие соединения
					|| !operatorNode->hasConnection(1)
					|| !operatorNode->hasConnection(2))
					return false;
				if (operatorNode->connectionType(0) != node::Variable
					|| operatorNode->connectionType(1) != node::Variable
					|| operatorNode->connectionType(2) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(0)
					!= operatorNode->connectionSubtype(1)
					|| operatorNode->connectionSubtype(0)
					!= operatorNode->connectionSubtype(2))
					return false;//соответствие типов данных
				if (operatorNode->connectionSubtype(0) == baseValueType::Bool)
					return false;
				break;

			case  Multiplication://////////////////////////////////////////////
				if (!operatorNode->hasConnection(0)//наличие соединения
					|| !operatorNode->hasConnection(1)
					|| !operatorNode->hasConnection(2))
					return false;
				if (operatorNode->connectionType(0) != node::Variable
					|| operatorNode->connectionType(1) != node::Variable
					|| operatorNode->connectionType(2) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(0)
					!= operatorNode->connectionSubtype(1)
					|| operatorNode->connectionSubtype(0)
					!= operatorNode->connectionSubtype(2))
					return false;//соответствие типов данных
				if (operatorNode->connectionSubtype(0) == baseValueType::Bool)
					return false;
				break;

			case  Division://////////////////////////////////////////////
				if (!operatorNode->hasConnection(0)//наличие соединения
					|| !operatorNode->hasConnection(1)
					|| !operatorNode->hasConnection(2))
					return false;
				if (operatorNode->connectionType(0) != node::Variable
					|| operatorNode->connectionType(1) != node::Variable
					|| operatorNode->connectionType(2) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(0)
					!= operatorNode->connectionSubtype(1)
					|| operatorNode->connectionSubtype(0)
					!= operatorNode->connectionSubtype(2))
					return false;//соответствие типов данных
				if (operatorNode->connectionSubtype(0) == baseValueType::Bool)
					return false;
				break;
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			case  Equal://////////////////////////////////////////////
				if (!operatorNode->hasConnection(0)//наличие соединения
					|| !operatorNode->hasConnection(1)
					|| !operatorNode->hasConnection(2))
					return false;
				if (operatorNode->connectionType(0) != node::Variable
					|| operatorNode->connectionType(1) != node::Variable
					|| operatorNode->connectionType(2) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(1)
					!= operatorNode->connectionSubtype(2))
					return false;//соответствие типов аргументов
				if (operatorNode->connectionSubtype(0) >= baseValueType::Float)
					return false;//только целочисленное возвращаемое значение
				break;

			case  NotEqual://////////////////////////////////////////////
				if (!operatorNode->hasConnection(0)//наличие соединения
					|| !operatorNode->hasConnection(1)
					|| !operatorNode->hasConnection(2))
					return false;
				if (operatorNode->connectionType(0) != node::Variable
					|| operatorNode->connectionType(1) != node::Variable
					|| operatorNode->connectionType(2) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(1)
					!= operatorNode->connectionSubtype(2))
					return false;//соответствие типов аргументов
				if (operatorNode->connectionSubtype(0) >= baseValueType::Float)
					return false;//только целочисленное возвращаемое значение
				break;

			case  Less:		//////////////////////////////////////////////
				if (!operatorNode->hasConnection(0)//наличие соединения
					|| !operatorNode->hasConnection(1)
					|| !operatorNode->hasConnection(2))
					return false;
				if (operatorNode->connectionType(0) != node::Variable
					|| operatorNode->connectionType(1) != node::Variable
					|| operatorNode->connectionType(2) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(1)
					!= operatorNode->connectionSubtype(2))
					return false;//соответствие типов аргументов
				if (operatorNode->connectionSubtype(0) >= baseValueType::Float)
					return false;//только целочисленное возвращаемое значение
				break;
			case  Greater:		//////////////////////////////////////////////
				if (!operatorNode->hasConnection(0)//наличие соединения
					|| !operatorNode->hasConnection(1)
					|| !operatorNode->hasConnection(2))
					return false;
				if (operatorNode->connectionType(0) != node::Variable
					|| operatorNode->connectionType(1) != node::Variable
					|| operatorNode->connectionType(2) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(1)
					!= operatorNode->connectionSubtype(2))
					return false;//соответствие типов аргументов
				if (operatorNode->connectionSubtype(0) >= baseValueType::Float)
					return false;//только целочисленное возвращаемое значение
				break;
			case  LessOrEqual://////////////////////////////////////////////
				if (!operatorNode->hasConnection(0)//наличие соединения
					|| !operatorNode->hasConnection(1)
					|| !operatorNode->hasConnection(2))
					return false;
				if (operatorNode->connectionType(0) != node::Variable
					|| operatorNode->connectionType(1) != node::Variable
					|| operatorNode->connectionType(2) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(1)
					!= operatorNode->connectionSubtype(2))
					return false;//соответствие типов аргументов
				if (operatorNode->connectionSubtype(0) >= baseValueType::Float)
					return false;//только целочисленное возвращаемое значение
				break;

			case  GreaterOrEqual://////////////////////////////////////////////
				if (!operatorNode->hasConnection(0)//наличие соединения
					|| !operatorNode->hasConnection(1)
					|| !operatorNode->hasConnection(2))
					return false;
				if (operatorNode->connectionType(0) != node::Variable
					|| operatorNode->connectionType(1) != node::Variable
					|| operatorNode->connectionType(2) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(1)
					!= operatorNode->connectionSubtype(2))
					return false;//соответствие типов аргументов
				if (operatorNode->connectionSubtype(0) >= baseValueType::Float)
					return false;//только целочисленное возвращаемое значение
				break;
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			case  LogicNegation://////////////////////////////////////////////
				if (!operatorNode->hasConnection(0)//наличие соединения
					|| !operatorNode->hasConnection(1))
					return false;
				if (operatorNode->connectionType(0) != node::Variable
					|| operatorNode->connectionType(1) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(0) >= baseValueType::Float)
					return false;//только целочисленное возвращаемое значение
				if (operatorNode->connectionSubtype(1) >= baseValueType::Float)
					return false;
				break;

			case  LogicAnd://////////////////////////////////////////////
				if (!operatorNode->hasConnection(0)//наличие соединения
					|| !operatorNode->hasConnection(1)
					|| !operatorNode->hasConnection(2))
					return false;
				if (operatorNode->connectionType(0) != node::Variable
					|| operatorNode->connectionType(1) != node::Variable
					|| operatorNode->connectionType(2) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(0) >= baseValueType::Float)
					return false;//только целочисленное возвращаемое значение
				if (operatorNode->connectionSubtype(1)
					!= operatorNode->connectionSubtype(2))
					return false;//соответствие типов аргументов
				break;

			case  LogicOr://////////////////////////////////////////////
				if (!operatorNode->hasConnection(0)//наличие соединения
					|| !operatorNode->hasConnection(1)
					|| !operatorNode->hasConnection(2))
					return false;
				if (operatorNode->connectionType(0) != node::Variable
					|| operatorNode->connectionType(1) != node::Variable
					|| operatorNode->connectionType(2) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(0) >= baseValueType::Float)
					return false;//только целочисленное возвращаемое значение
				if (operatorNode->connectionSubtype(1)
					!= operatorNode->connectionSubtype(2))
					return false;//соответствие типов аргументов
				break;

			case  LogicExclusive://////////////////////////////////////////////
				if (!operatorNode->hasConnection(0)//наличие соединения
					|| !operatorNode->hasConnection(1)
					|| !operatorNode->hasConnection(2))
					return false;
				if (operatorNode->connectionType(0) != node::Variable
					|| operatorNode->connectionType(1) != node::Variable
					|| operatorNode->connectionType(2) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(0) >= baseValueType::Float)
					return false;//только целочисленное возвращаемое значение
				if (operatorNode->connectionSubtype(1)
					!= operatorNode->connectionSubtype(2))
					return false;//соответствие типов аргументов
				break;
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			case  Increment://////////////////////////////////////////////
				if (!operatorNode->hasConnection(0))//наличие соединения
					return false;
				if (operatorNode->connectionType(0) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(0) >= baseValueType::Float)
					return false;//только целочисленное возвращаемое значение
				if (operatorNode->connectionSubtype(0) == baseValueType::Bool)
					return false;
				break;
			case  Decrement://////////////////////////////////////////////
				if (!operatorNode->hasConnection(0))//наличие соединения
					return false;
				if (operatorNode->connectionType(0) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(0) >= baseValueType::Float)
					return false;//только целочисленное возвращаемое значение
				if (operatorNode->connectionSubtype(0) == baseValueType::Bool)
					return false;
				break;
			default:
				throw;
			}
			return true;
		}
	}

	/*
	switch (toType)
			{
			case baseValueType::Bool:

				break;
			case baseValueType::Int8:

				break;
			case baseValueType::uInt8:

				break;
			case baseValueType::Int16:

				break;
			case baseValueType::uInt16:

				break;
			case baseValueType::Int32:

				break;
			case baseValueType::uInt32:

				break;
			case baseValueType::Int64:

				break;
			case baseValueType::uInt64:

				break;
			case baseValueType::Float:

				break;
			case baseValueType::Double:

				break;
			}*/
}
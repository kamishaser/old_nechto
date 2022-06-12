#pragma once
#include "node.h"
#include "baseValueTypes.h"

namespace nechto
{
	namespace mathOperator
	{
		enum Types
		{
			Error,
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
			Decrement,		// --
		};
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool isCorrect(nodePtr operatorNode);//проверка корректности узла оператора

		template <class TCon>
		size_t unaryMinus(void* firstValue)
		{
			size_t temp = 0;
			void* temptr = &temp;
			*static_cast<TCon*>(temptr) = -(*static_cast<TCon*>(firstValue));
			return temp;
		}
		template <class TCon>
		size_t logicNegation(void* firstValue)
		{
			size_t temp = 0;
			void* temptr = &temp;
			*static_cast<int64_t*>(temptr) = !(*static_cast<TCon*>(firstValue));
			return temp;
		}

		template <class TCon>
		size_t addition(void* firstValue, void* secondValue)
		{
			size_t temp = 0;
			void* temptr = &temp;
			*static_cast<TCon*>(temptr) =
				(*static_cast<TCon*>(firstValue)) + (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t substraction(void* firstValue, void* secondValue)
		{
			size_t temp = 0; temp;
			void* temptr = &temp;
			*static_cast<TCon*>(temptr) =
				(*static_cast<TCon*>(firstValue)) - (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t multiplication(void* firstValue, void* secondValue)
		{
			size_t temp = 0;
			void* temptr = &temp;
			*static_cast<TCon*>(temptr) =
				(*static_cast<TCon*>(firstValue)) * (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t division(void* firstValue, void* secondValue)
		{
			size_t temp = 0;
			void* temptr = &temp;
			*static_cast<TCon*>(temptr) =
				(*static_cast<TCon*>(firstValue)) / (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t equal(void* firstValue, void* secondValue)
		{
			size_t temp = 0;
			void* temptr = &temp;
			*static_cast<int64_t*>(temptr) =
				(*static_cast<TCon*>(firstValue)) == (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t notEqual(void* firstValue, void* secondValue)
		{
			size_t temp = 0;
			void* temptr = &temp;
			*static_cast<int64_t*>(temptr) =
				(*static_cast<TCon*>(firstValue)) != (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t less(void* firstValue, void* secondValue)
		{
			size_t temp = 0;
			void* temptr = &temp;
			*static_cast<int64_t*>(temptr) =
				(*static_cast<TCon*>(firstValue)) < (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t greater(void* firstValue, void* secondValue)
		{
			size_t temp = 0;
			void* temptr = &temp;
			*static_cast<int64_t*>(temptr) =
				(*static_cast<TCon*>(firstValue)) > (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t lessOrEqual(void* firstValue, void* secondValue)
		{
			size_t temp = 0;
			void* temptr = &temp;
			*static_cast<int64_t*>(temptr) =
				(*static_cast<TCon*>(firstValue)) <= (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t greaterOrEqual(void* firstValue, void* secondValue)
		{
			size_t temp = 0;
			void* temptr = &temp;
			*static_cast<int64_t*>(temptr) =
				(*static_cast<TCon*>(firstValue)) >= (*static_cast<TCon*>(secondValue));
			return temp;
		}

		template <class TCon>
		size_t logicAnd(void* firstValue, void* secondValue)
		{
			size_t temp = 0;
			void* temptr = &temp;
			*static_cast<int64_t*>(temptr) =
				(*static_cast<TCon*>(firstValue)) && (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t logicOr(void* firstValue, void* secondValue)
		{
			size_t temp = 0;
			void* temptr = &temp;
			*static_cast<int64_t*>(temptr) =
				(*static_cast<TCon*>(firstValue)) || (*static_cast<TCon*>(secondValue));
			return temp;
		}
		template <class TCon>
		size_t logicExclusive(void* firstValue, void* secondValue)
		{
			size_t temp = 0;
			void* temptr = &temp;
			*static_cast<int64_t*>(temptr) =
				(*static_cast<TCon*>(firstValue)) ^ (*static_cast<TCon*>(secondValue));
			return temp;
		}

		template <class TCon>
		size_t increment(void* firstValue)
		{
			size_t temp = 0;
			void* temptr = &temp;
			*static_cast<TCon*>(temptr) = *static_cast<TCon*>(firstValue) + 1;
			return temp;
		}
		template <class TCon>
		size_t decrement(void* firstValue)
		{
			size_t temp = 0;
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
				case baseValueType::Int64:
					temp = unaryMinus<int64_t>(&firstValue);
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
				case baseValueType::Int64:
					temp = addition<int64_t>(&firstValue, &secondValue);
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
				case baseValueType::Int64:
					temp = substraction<int64_t>(&firstValue, &secondValue);
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
				case baseValueType::Int64:
					temp = multiplication<int64_t>(&firstValue, &secondValue);
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
				case baseValueType::Int64:
					temp = division<int64_t>(&firstValue, &secondValue);
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
				case baseValueType::Int64:
					temp = equal<int64_t>(&firstValue, &secondValue);
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
				if (resultType != baseValueType::Int64)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Int64, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  NotEqual:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Int64:
					temp = notEqual<int64_t>(&firstValue, &secondValue);
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
				if (resultType != baseValueType::Int64)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Int64, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  Less:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Int64:
					temp = less<int64_t>(&firstValue, &secondValue);
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
				if (resultType != baseValueType::Int64)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Int64, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  Greater:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Int64:
					temp = greater<int64_t>(&firstValue, &secondValue);
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
				if (resultType != baseValueType::Int64)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Int64, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  LessOrEqual:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Int64:
					temp = lessOrEqual<int64_t>(&firstValue, &secondValue);
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
				if (resultType != baseValueType::Int64)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Int64, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  GreaterOrEqual:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Int64:
					temp = greaterOrEqual<int64_t>(&firstValue, &secondValue);
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
				if (resultType != baseValueType::Int64)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Int64, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			case  LogicNegation:
				firstValue = operatorNode->connection[1].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Int64:
					temp = logicNegation<int64_t>(&firstValue);
					break;	
				default:
					throw;
				}
				if (resultType != baseValueType::Int64)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Int64, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  LogicAnd:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Int64:
					temp = logicAnd<int64_t>(&firstValue, &secondValue);
					break;
				default:
					throw;
				}
				if (resultType != baseValueType::Int64)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Int64, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  LogicOr:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Int64:
					temp = logicOr<int64_t>(&firstValue, &secondValue);
					break;
				default:
					throw;
				}
				if (resultType != baseValueType::Int64)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Int64, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  LogicExclusive:
				firstValue = operatorNode->connection[1].load()->data.load();
				secondValue = operatorNode->connection[2].load()->data.load();
				comVarType = operatorNode->connection[1].load()->subtype;
				switch (comVarType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Int64:
					temp = logicExclusive<int64_t>(&firstValue, &secondValue);
					break;
				default:
					throw;
				}
				if (resultType != baseValueType::Int64)//по необходимости преобразование типа
					typeCast(&temp, &temp, baseValueType::Int64, resultType);
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			case  Increment:
				firstValue = operatorNode->connection[0].load()->data.load();
				switch (resultType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Int64:
					temp = increment<int64_t>(&firstValue);
					break;
				default:
					throw;
				}
				operatorNode->connection[0].load()->data.store(temp);//запись результата
				break;
			case  Decrement:
				firstValue = operatorNode->connection[0].load()->data.load();
				switch (resultType)
				{//выбор используемой функции в зависимости от типа данных
				case baseValueType::Int64:
					temp = decrement<int64_t>(&firstValue);
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
			if (!operatorNode->hasConnection(0))
				return false;
			if (operatorNode->connectionSubtype(0) == baseValueType::NodePtr)
				return false;
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
				break;

			case  Division://////////////////////////////////////////////
				if (   !operatorNode->hasConnection(0)//наличие соединения
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
				break;
			case  Decrement://////////////////////////////////////////////
				if (!operatorNode->hasConnection(0))//наличие соединения
					return false;
				if (operatorNode->connectionType(0) != node::Variable)
					return false;//проверка типа (только переменные)
				if (operatorNode->connectionSubtype(0) >= baseValueType::Float)
					return false;//только целочисленное возвращаемое значение
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
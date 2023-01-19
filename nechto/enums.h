#pragma once

namespace nechto
{
	namespace nodeT
	{
		enum Type : unsigned char//список типов нод
		{
			Deleted
			= 0,	//удалённый, несуществующий объект
			Hub
			= 1,	//разветвитель
			Group
			= 2,	//группа объектов
			Iterator
			= 3,	//указатель на объект
			Struct
			= 4,	//структурная нода
			Variable
			= 5,	//объект-переменная базового типа
			Vector
			= 6,	//вектор<subtype>
			Object
			= 7,	//внешний объект
			Text
			= 8,	//текст
			Operator
			= 9,	//оператор

			Method
			= 128,	//метод Object
			Condition
			= 129,	//if
			MathOperation
			= 130,	//математическая операция
			VectorOperation
			= 131,	//векторная операция
			ConnectionOperation
			= 132,	//операция подключения
			CreationOperation
			= 133,	//операция создания
			IterationOperation
			= 134,	//операция перемещения итератора
			HubManagement
			= 135,	//операции с хабами
			OperatorManagement
			= 136,	//управление оператором
			ReadingOperation
			= 137,	//операция считывание информации
			TextOperation
			= 138	//операция с текстом

		};//до 127 - объекты, после - операции
	}
	namespace variableT
	{
		enum Type
		{
			F64 = 0,//false
			I64 = 1 //true
		};
	}
	namespace objectT
	{
		enum Type
		{
			NotOwning = 0,//false
			Owning = 1 //true
		};
	}
	namespace textT
	{
		enum Type
		{
			NotOwning = 0,//false
			Owning = 1 //true
		};
	}
	namespace groupT
	{
		enum Type
		{
			weak = 0,//false
			strong = 1 //true
		};
	}
	namespace MathOperationT
	{
		enum Type
		{
			Assigment,		// =
			UnaryMinus,		// 0-

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

			Increment,		// ++
			Decrement,		// --
		};
	}
	namespace iteratorT
	{
		enum Type
		{
			PortIter,//итератор соединений
			GroupIter//итератор массива
		};
	}
	namespace vectorT
	{
		enum Type
		{
			n
		};
	}
	namespace operatorT
	{
		enum Type
		{

		};
	}
	namespace vectorOperationT
	{
		enum Type
		{
			n
		};
	}
	namespace connectionOperationT
	{
		enum Type
		{
			n
		};
	}
	namespace creationOperationT
	{
		enum Type
		{
			n
		};
	}
	namespace iterationOperationT
	{
		enum Type
		{
			n
		};
	}
	namespace hubManagementT
	{
		enum Type
		{
			n
		};
	}
	namespace operatorManagementT
	{
		enum Type
		{
			n
		};
	}
	namespace readingOperationT
	{
		enum Type
		{
			n
		};
	}
	namespace textOperationT
	{
		enum Type
		{
			n
		};
	}
	namespace structT
	{
		enum Type
		{
			n,
		};
	}
	namespace entityT
	{
		enum Type
		{
			oneSideLink,
			singleConnection,
			multipleConnection
		};
	}


	enum class contentRule
	{
		Any,
		Empty,
		NonEmpty
	};
}
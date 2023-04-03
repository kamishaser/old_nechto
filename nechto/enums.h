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
			= 2,	//группа нод
			Struct
			= 3,	//структурная нода
			Pointer
			= 4,	//указатель на порт
			Path
			= 5,	//путь до ноды
			Variable
			= 6,	//объект-переменная базового типа
			Vector
			= 7,	//вектор<subtype>
			Entity
			= 8,	//внешняя сущность
			Text
			= 9,	//текст
			Operator
			= 10,	//оператор

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
			PointerationOperation
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
	namespace pointerT
	{
		enum Type
		{
			PortPointer,//итератор соединений
			GroupPointer//итератор массива
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
			Disconnect,
			Connect,
			Swap,
		};
	}
	namespace creationOperationT
	{
		enum Type
		{
			DeleteNode,
			CreateNode,
			CopyNode,
			CreateGroup,
			CreatePointer,
			CreateStruct,
			CreateVariable,
			CreateVector,

			CreateEntity,
			CreateText,
			CreateOperator,
			
			CreateMethod,
			CreateMathOperation,
			CreateVectorOperation,
			CreateConnectionOperation,
			CreateCreationOperation,
			CreatePointerationOperation,
			CreateHubManagementOperation,
			CreateOperatorManagementOperation,
			CreateReadingOperation,
			CreateTextOperation,
			
		};
	}
	namespace pointerMovementOperationT
	{
		enum Type
		{
			stepForward,
			stepBack, //только для групп
			goToBegin,
			goToPosition,
			nextConnectedPort,
			
			setByPointer,
			setByPointerPurpose, //переименовать при первой же возможности. Ну очень тупое название
			setByConnection,
			
			assigment,
			nearestBackPort,
			lastConnectedPort,
			
			firstEmptyPort,
			firstEmtpyHubPort,
			reversePort,
			emptyPortAtFirstHub,
			findNearestConnection,
			emtpyAfterCurrent,
		};
	}
	namespace hubManagementT
	{
		enum Type
		{
			insert,
			erase,
			compress,
			compressGroup
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
			pointerExist,//наличие у указателя hubPosPair
			pointerdExist,//pointer.get().exist
			connectionExist,
			isConnection,
			isGroupMember,

			numberOfHubs,
			numberOfConnectedPorts,
			numberOfGroupHubs,
			numberOfGroupMembers,

			getPointerGlobalPosition,
			getPointerLocalPosition,
			isEqual,
			isEqualPurpese,

			isPositionEqual,
			isPositionLess,

			entityExist,
			
			isTypeEquals,
			isTypeSubtypeEquals,
			getSubtypeId,
			getTypeId,

			isCorrectOperation, //все соединения подключены верно
			isCorrectConnection, //одно соединение подключено верно


		};
	}
	namespace textOperationT
	{
		enum Type
		{
			assigment,
			addition, //? сложение 2 строк с получением удлинённой
			substring, //получение подкстроки из исходной

			size,
			constains,
			compare,
			startWith,
			endWith,

			getTypeName,
			getSubtypeName,
			getEssenceTypeName
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
#pragma once
#include <cassert>
namespace nechto
{
	using ushort = unsigned short;
	using i64 = long long;
	using f64 = double;

	namespace nodeStorage
	{
		class threadLocalAllocator;
		class nodeAllocator;
	}
	class nodePtr;
	class connecter;
	class creator;

	struct nodeId//пришлось запрятать сюда для защиты от дурака
	{
	public:
		ushort first = 0;
		ushort second = 0;

		ushort getFirst() const
		{
			return first;
		}
		ushort getSecond() const
		{
			return second;
		}

		bool operator == (nodeId another) const
		{
			return ((first == another.first) && (second == another.second));
		}
		bool operator != (nodeId another) const
		{
			return !(*this == another);
		}
		operator bool() const
		{
			return exist();
		}
		bool exist() const
		{
			return (first && second);
		}
		friend class nodeStorage::threadLocalAllocator;
		friend class nodeStorage::nodeAllocator;
		friend class nodePtr;
		friend class nodeData;

		void operator=(nullptr_t)
		{
			first = 0;
			second = 0;
		}
		bool operator==(nullptr_t) const
		{
			return !exist();
		}
		bool operator!=(nullptr_t) const
		{
			return exist();
		}
	private:
		nodeId(ushort f, ushort s)
			:first(f), second(s) {}
		nodeId() {}
	};
	struct nodeData
	{
		i64 data = 0;
		char type = 0;
		char subtype = 0;

		nodeId port[4];
		nodeId hubPort;
	};
	template<class TCon>
	concept existOrNonExist =
		requires(TCon a) 
	{
		a.exist();
	};

	template <existOrNonExist TCon>
	class existing : public TCon
	{
	public:
		existing<TCon>(const TCon& tCon)
			:TCon(tCon)
		{
			assert(tCon.exist());
		}
	};
	namespace nodeT
	{
		enum Type :char//список типов нод
		{
			Deleted,
			Hub,					//разветвитель
			Variable,				//объект-переменная базового типа
			Math,					//математический оператор
			Condition,				//if
			ExternalObject,			//внешний объект
			Method,					//метод externalObject
			Pointer,				//указатель на объект
			Group,					//группа объектов
			Text,					//текст
			NodeOperator,			//узловой оператор
		};
	}

	class nullNodePtrT;

	class i64VariablePtr;
	class f64VariablePtr;
	class variablePtr;
	class pointerPtr;
	class simplePointerPtr;
	class iteratorPtr;
	class conIteratorPtr;
	class groupIteratorPtr;
	class groupPtr;
	class methodPtr;
	class mathPtr;
	class exteranalObjectPtr;

	namespace variableT
	{
		enum Type :char
		{
			F64 = 0,//false
			I64 = 1 //true
		};
	}
	namespace mathOperatorT
	{
		enum Type :char
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
		enum Type :char
		{
			Simple = 0,//одиночная ссылка
			ConIter,//итератор соединений
			GroupIter//итератор массива
		};
	}
}
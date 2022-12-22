#pragma once
#include <cassert>
#include <string>
namespace nechto
{
	using ushort = unsigned short;
	using i64 = long long;
	using f64 = double;
	using ui32 = unsigned long;
	using ustr = std::u16string;

	namespace nodeStorage
	{
		class threadLocalAllocator;
		class nodeAllocator;
	}
	class nodePtr;
	class connecter;
	class creator;
	class hubManager;
	class compressor;

	struct iterator;
	class portIterator;
	class groupIterator;

	class serializer;
	class deserializer;

	//������� ����� �������������� ����
	struct nodeId
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
		friend struct nodeData;

		void operator=(std::nullptr_t)
		{
			first = 0;
			second = 0;
		}
		bool operator==(std::nullptr_t) const
		{
			return !exist();
		}
		bool operator!=(std::nullptr_t) const
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

	template <class TCon>
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
		enum Type :char//������ ����� ���
		{
			Deleted,
			Hub = 1,				//������������
			Group = 2,				//������ ��������
			Pointer = 3,			//��������� �� ������
			Variable = 4,			//������-���������� �������� ����
			Object = 5,		//������� ������
			Text = 6,				//�����

			MathOperator = 65,		//�������������� ��������
			Condition = 66,			//if
			Method = 67,			//����� Object
			
			NodeOperator,			//������� ��������
		};
	}

	class nullNodePtrT;

	class i64VariablePtr;
	class f64VariablePtr;
	class variablePtr;
	class pointerPtr;
	class simplePointerPtr;
	class iteratorPtr;
	class portIteratorPtr;
	class groupIteratorPtr;
	class textPtr;
	class groupPtr;
	class methodPtr;
	class mathOperatorPtr;
	class nonTypedObjectPtr;
	class objectNullPtr;

	class namedExCon;
	class namedExConGroup;

	namespace variableT
	{
		enum Type :char
		{
			F64 = 0,//false
			I64 = 1 //true
		};
	}
	namespace objectT
	{
		enum Type :char
		{
			NotOwning = 0,//false
			Owning = 1 //true
		};
	}
	namespace textT
	{
		enum Type :char
		{
			NotOwning = 0,//false
			Owning = 1 //true
		};
	}
	namespace groupT
	{
		enum Type :char
		{
			weak = 0,//false
			strong = 1 //true
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
			Simple = 0,//��������� ������
			ConIter,//�������� ����������
			GroupIter//�������� �������
		};
	}
	bool hasSubType(char type)
	{
		switch (type)
		{
		case nodeT::Hub:
		case nodeT::Condition:
		case nodeT::Group:
			return false;
		default:
			return true;
		}
	}
	bool hasData(char type)
	{
		switch (type)
		{
		case nodeT::Variable:
		case nodeT::Text:
		case nodeT::Group:
		case nodeT::Pointer:
			return true;
		default:
			return false;
		}
	}
}
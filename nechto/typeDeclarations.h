#pragma once
#include <cassert>
#include <string>
#include <iostream>
namespace nechto
{
	using ushort = unsigned short;
	using i64 = long long;
	using f64 = double;
	using ui32 = unsigned long;

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
	class nodeOperator;

	struct iterator;
	class portIterator;
	class groupIterator;

	class garbageCollector;

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
		bool exist() const
		{
			return first;
		}
		friend class nodeStorage::threadLocalAllocator;
		friend class nodeStorage::nodeAllocator;
		friend class nodePtr;
		friend class garbageCollector;
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
		bool operator<(const nodeId id) const
		{
			if (first > id.first)
				return false;
			if (first < id.first)
				return true;
			if (second < id.second)
				return true;
			return false;

		}
	private:
		nodeId(ushort f, ushort s)
			:first(f), second(s) {}
		nodeId() {}
	};
	struct nodeData
	{
		i64 data = 0;
		ushort bottomLevel = 0;
		unsigned char type = 0;
		unsigned char subtype = 0;
		nodeId hubPort;
		nodeId port[4];
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
		
		enum Type : unsigned char//������ ����� ���
		{
			Deleted
			= 0,	//��������, �������������� ������
			Hub 
			= 1,	//������������
			Group 
			= 2,	//������ ��������
			Iterator 
			= 3,	//��������� �� ������
			Struct 
			= 4,	//����������� ����
			Variable 
			= 5,	//������-���������� �������� ����
			Vector
			= 6,	//������<subtype>
			Object 
			= 7,	//������� ������
			Text 
			= 8,	//�����
			Operator
			= 9,	//��������

			MathOperation
			= 128,	//�������������� ��������
			VectorOperation
			= 129,	//��������� ��������
			Condition 
			= 130,	//if
			Method 
			= 131,	//����� Object
			ConnectionOperation
			= 132,	//�������� �����������
			CreationOperation
			= 133,	//�������� ��������
			IterationOperation
			= 134,	//�������� ����������� ���������
			HubManagement
			= 135,	//�������� � ������
			OperatorManagement
			= 136,	//���������� ����������
			ReadingOperation
			= 137,	//�������� ���������� ����������
			TextOperation
			= 138	//�������� � �������

		};//�� 127 - �������, ����� - ��������
	}

	class nullNodePtrT;

	class i64VariablePtr;
	class f64VariablePtr;
	class variablePtr;
	class iteratorPtr;
	class portIteratorPtr;
	class groupIteratorPtr;
	class textPtr;
	class groupPtr;
	class methodPtr;
	class MathOperationPtr;
	class nonTypedObjectPtr;
	class objectNullPtr;

	class namedExCon;
	class namedExConGroup;

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
			PortIter,//�������� ����������
			GroupIter//�������� �������
		};
	}
	bool hasSubType(unsigned char type)
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
	bool hasData(unsigned char type)
	{
		switch (type)
		{
		case nodeT::Variable:
		case nodeT::Text:
		case nodeT::Group:
		case nodeT::Iterator:
			return true;
		default:
			return false;
		}
	}
	bool isOperation(unsigned char type)
	{
		return(type > 127);
	}
	void pConList(nodePtr v1, std::wstring header = std::wstring(L"list of connections"));
}
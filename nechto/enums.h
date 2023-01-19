#pragma once

namespace nechto
{
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

			Method
			= 128,	//����� Object
			Condition
			= 129,	//if
			MathOperation
			= 130,	//�������������� ��������
			VectorOperation
			= 131,	//��������� ��������
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
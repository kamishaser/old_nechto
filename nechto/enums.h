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
			= 2,	//������ ���
			Struct
			= 3,	//����������� ����
			Pointer
			= 4,	//��������� �� ����
			Path
			= 5,	//���� �� ����
			Variable
			= 6,	//������-���������� �������� ����
			Vector
			= 7,	//������<subtype>
			Entity
			= 8,	//������� ��������
			Text
			= 9,	//�����
			Operator
			= 10,	//��������

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
			PointerationOperation
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
			PortPointer,//�������� ����������
			GroupPointer//�������� �������
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
			stepBack, //������ ��� �����
			goToBegin,
			goToPosition,
			nextConnectedPort,
			
			setByPointer,
			setByPointerPurpose, //������������� ��� ������ �� �����������. �� ����� ����� ��������
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
			pointerExist,//������� � ��������� hubPosPair
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

			isCorrectOperation, //��� ���������� ���������� �����
			isCorrectConnection, //���� ���������� ���������� �����


		};
	}
	namespace textOperationT
	{
		enum Type
		{
			assigment,
			addition, //? �������� 2 ����� � ���������� ���������
			substring, //��������� ���������� �� ��������

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
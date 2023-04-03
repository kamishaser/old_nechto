#pragma once
#include "nodePtr.h"
#include <string>
#include <array>

namespace nechto
{
	struct elementaryRule
	{
		unsigned char type = 0; //��� ����������. ���� null - ���������� �� �����������
		bool subtypeImportant = 0;
		unsigned char subtype = 0;
		bool transitable = true; //����� �� ���������� ����� ��������. �� �������� � ������ �����������
	};
	struct pointerRule
	{
		bool subtypeImportant = 0;
		unsigned char subtype = 0;
		bool necessarilyExistingPointer = true;
		contentRule ptrContentRule = contentRule::Any;
	};
	struct nonTypedEntityRule
	{
		bool subtypeImportant = 0;
		unsigned char subtype = 0;
		unsigned char nodeType = nodeT::Entity;
		bool transitable = true; //����� �� ���������� ����� ��������. �� �������� � ������ �����������
		contentRule entityContentRule = contentRule::Any;
	};
	struct entityRule
	{
		bool subtypeImportant = 0;
		unsigned char subtype = 0;
		bool transitable = true; //����� �� ���������� ����� ��������. �� �������� � ������ �����������
	};
	class conRuleInterface
	{
	protected:
	public:
		virtual nodePtr getConnection(nodePtr node)const//���� - �������� �����
		{
			return nullptr;
		}
		virtual bool check(nodePtr node) const//���� - �������� �����
		{
			return false;
		}
		//�������� �� �������� ������������ � ���������� ��� ������� ������
		virtual bool constant(nodePtr node) const//� �������� �� �����������
		{
			return false;
		}
		virtual ~conRuleInterface()
		{}
	};
	using ccrInterface = const conRuleInterface*;
	namespace cr
	{
		ccrInterface getNoneRule();
		ccrInterface None = getNoneRule();
	}

	struct conRule
	{
		std::array<const conRuleInterface*, 3> conRules;//������� ��� �����������
		std::array<bool, 3> read;
		std::array<bool, 3> write;
		conRule(
			ccrInterface port0 = cr::None, bool read0 = false, bool write0 = false,
			ccrInterface port1 = cr::None, bool read1 = false, bool write1 = false,
			ccrInterface port2 = cr::None, bool read2 = false, bool write2 = false)
			:conRules{ port0, port1, port2 },
			read{ read0, read1, read2 },
			write{ write0, write1, write2 } {}

	};
}
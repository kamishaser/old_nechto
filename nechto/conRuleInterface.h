#pragma once
#include "nodePtr.h"

namespace nechto
{
	struct elementaryRule
	{
		unsigned char type = 0; //��� ����������. ���� null - ���������� �� �����������
		bool subtypeImportant = 0;
		unsigned char subtype = 0;
	};
	struct iteratorRule
	{
		bool noTransit = 0;
		bool necessarilyPointToConnectedPort = true;
		bool necessarilyPointToNoConnectedPort = false;
	};
	struct entityRule
	{
		bool typeImportant = false;
		bool necessarilyEmpty = false;
		bool necessarilyNoEmpty = false;
		std::wstring typeName;
		i64 id = 0;
	};
	class conRuleInterface
	{
	protected:
	public:
		virtual nodePtr getConnection(existing<nodePtr> v1, int number)const
		{
			return nullptr;
		}
		virtual bool check(nodePtr v1) const
		{
			return false;
		}
		virtual elementaryRule getElementaryRule(int number) const//�������� ������ ��� ������������ �������
		{
			return elementaryRule();
		}
		virtual iteratorRule getIteratorRule(int number) const//�������� ������ ��� ����������
		{
			return iteratorRule();
		}
		virtual entityRule getEntityRule(int number) const//�������� ������ ��� ���������
		{
			return entityRule();
		}
		virtual bool isStandartRule(int number) const
		{
			return true;
		}

		virtual ~conRuleInterface()
		{}
	};
}
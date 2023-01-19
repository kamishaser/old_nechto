#pragma once
#include "nodePtr.h"
#include <string>

namespace nechto
{
	struct elementaryRule
	{
		unsigned char type = 0; //тип соединения. Если null - соединение не обязательно
		bool subtypeImportant = 0;
		unsigned char subtype = 0;
		bool transitable = true; //можно ли передавать через итератор. Не работает с самими итераторами
	};
	struct iteratorRule
	{
		bool subtypeImportant = 0;
		unsigned char subtype = 0;
		bool necessarilyExistingIterator = true;
		contentRule iterContentRule = contentRule::Any;
	};
	struct nonTypedEntityRule
	{
		bool subtypeImportant = 0;
		unsigned char subtype = 0;
		unsigned char nodeType = nodeT::Object;
		bool transitable = true; //можно ли передавать через итератор. Не работает с самими итераторами
		contentRule entityContentRule = contentRule::Any;
	};
	struct entityRule
	{
		bool subtypeImportant = 0;
		unsigned char subtype = 0;
		bool transitable = true; //можно ли передавать через итератор. Не работает с самими итераторами
	};
	class conRuleInterface
	{
	protected:
	public:
		virtual nodePtr getConnection(nodePtr node)const//нода - значение порта
		{
			return nullptr;
		}
		virtual bool check(nodePtr node) const//нода - значение порта
		{
			return false;
		}
		virtual ~conRuleInterface()
		{}
	};

	struct conRule
	{
		const conRuleInterface* conRules[3];//правила трёх подключений
		bool read[3] = { true, true, true };
		bool write[3] = { true, true, true };
		bool changeNet = true;
	};
	struct operation
	{
		bool(*operationPtr)(nodePtr, nodePtr, nodePtr);
		conRule cRule;
		std::wstring name;

		operation(const std::wstring& n, const conRule& cr,
			bool(*op)(nodePtr, nodePtr, nodePtr) = nullptr)
			:operationPtr(op), cRule(cr), name(n) {}

		operation(const std::wstring& n, const conRuleInterface* con0, const conRuleInterface* con1, const conRuleInterface* con2,
			bool(*op)(nodePtr, nodePtr, nodePtr) = nullptr)
			:operation(n, conRule(con0, con1, con2), op) {}
	};
}
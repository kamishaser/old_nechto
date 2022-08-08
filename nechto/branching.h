#pragma once
#include "node.h"
#include "mathOperator.h"
#include "pointer.h"
#include "connectionIterator.h"

namespace nechto
{}
//оператор ветвления
//в данной версии nechto только условное ветвление
namespace nechto::branching
{
	//к соединению 0 подключается условие (напрямую либо через указатель)
	//к ссоединениям 1 и 2 следующие действия (true, false)
	
	bool check(nodePtr v1)
	{
		assert(typeCompare(v1, node::ConditionalBranching));
		nodePtr condition = v1->connection[0];
		if (!condition.exist())
			return false;
		if (condition->getType() == node::Pointer)
		{//доступ к условию не напрямую, а через указатель
			condition == pointer::follow(condition);
			if (!condition.exist())
				return false;
		}
		if (condition->getType() != node::Variable)
			return false;
		if (!condition->getSubtype())//в условии только int
			return false;
		return true;
	}
	//выполнение ветвления. Возвращает 0, если ошибка, иначе номер следующей ноды
	char perform(nodePtr v1)
	{
		nodePtr condition = v1->connection[0];
		if (condition->getType() == node::Pointer)
		{//доступ к условию не напрямую, а через указатель
			condition == pointer::follow(condition);
			if (!condition.exist())
				return 0;
		}
		if (condition->getType() != node::Variable)
			return 0;
		if (!condition->getSubtype())//в условии только int
			return 0;
		return condition->data;
	}
}
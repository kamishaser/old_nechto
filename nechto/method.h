#pragma once
#include "externalObject.h"

namespace nechto::method
{
	/*базовое правило подключения: 
	на первое подключение всегда ставится объект*/
	connectionRule defaultMmethodConnectionRule =
		connectionRule(conRule::ExternalObject, conRule::In_Output);

	externalObject* getObject(nodePtr methodNode)
	{
		nodePtr objectNode =
			defaultMmethodConnectionRule.getConnection(methodNode, 0);
		if (!objectNode.exist())
			return nullptr;
		return objectNode->getData<externalObject*>();
	}
	const operation& getMethod(nodePtr methodNode)
	{
		assert(typeCompare(methodNode, node::Method));
		externalObject* object = getObject(methodNode);
		if (object == nullptr)
			return operation::None;
		else
			return object->getMethod(methodNode->getSubtype());
	}
	const std::wstring getMethodName(nodePtr methodNode)
	{
		assert(typeCompare(methodNode, node::Method));
		externalObject* object = getObject(methodNode);
		if (object == nullptr)
			return std::wstring();
		else
			return object->getMethodName(methodNode->getSubtype());
	}
	bool operate(nodePtr methodNode)
	{
		return getMethod(methodNode).operate(methodNode);
	}
	bool check(nodePtr methodNode)
	{
		return getMethod(methodNode).cRule.check(methodNode);
	}
}
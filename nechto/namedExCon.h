#pragma once
#include "externalObject.h"

namespace nechto
{
	struct namedExCon :public externalObject
	{
		std::wstring name;
		namedExCon(const std::wstring& n)
			:externalObject(newExObjNode(0)), name(n)
			//при удалении ноды, удалится и сей объект !!!только выделять через new!!!
		{}
		virtual ~namedExCon()
		{
		}
		/*получение указателя на namedExCon по объекту.
		Возвращает nullptr при несоответствии*/
		static namedExCon* getByNode(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalObject)
				return nullptr;
			return dynamic_cast<namedExCon*>(v1->getData<externalObject*>());
		}
		const static std::wstring typeName;
		const static staticNodeOperationSet methodSet;
		const static connectionRule cRule;
		virtual const std::wstring& getTypeName() const override
		{
			return typeName;
		}
		virtual const operation& getMethod(char number)const override
		{
			return methodSet.getOperation(number);
		}
		virtual const conRule& getConnectionRule()const override
		{
			return cRule;
		}
	};
	const std::wstring namedExCon::typeName = L"nechtoIde.namedExCon";
	const connectionRule namedExCon::cRule = connectionRule{};
	const staticNodeOperationSet namedExCon::methodSet
	{
		/*namedOperation(L"", operation{
				connectionRule(conRule::ExternalObject, conRule::Input, nullptr,),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				return true;
			}}),*/
	};
}

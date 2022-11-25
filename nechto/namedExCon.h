#pragma once
#include "externalObject.h"

namespace nechto
{
	class namedExCon :public externalObject
	{
	protected:
		nodePtr exConNode;
		virtual void disconnect()
		{
			resetNode(exConNode);
			exConNode = nullptr;
		}
	public:
		const std::u16string name;
		namedExCon(externalObjectNullPtr emptyExObj, const std::u16string& n)
			:exConNode(emptyExObj), name(n) 
		{
			emptyExObj.set(this);
		}
		namedExCon(const std::u16string& n)
			:exConNode(creator::createExternalObject(this, false)), name(n) {}
		
		//отключение namedExCon от ноды
		
		virtual ~namedExCon()
		{
			disconnect();
		}
		externalObjectPtr<namedExCon> get() const
		{//внимание!!! Данная функция не может быть вызвана после disconnect!!!
			return nonTypedExternalObjectPtr(existing<nodePtr>(exConNode));
		}
		externalObjectPtr<namedExCon> operator->() const
		{
			return get();
		}

		const static externalObject::typeDefinition type;
		virtual const externalObject::typeDefinition& getType() const override
		{
			return type;
		}
	};
	const externalObject::typeDefinition namedExCon::type
	{ u"namedExCon" };
}

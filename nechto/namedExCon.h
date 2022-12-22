#pragma once
#include "object.h"

namespace nechto
{
	class namedExCon :public object
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
		namedExCon(objectNullPtr emptyExObj, const std::u16string& n)
			:exConNode(emptyExObj), name(n) 
		{
			emptyExObj.set(this);
		}
		namedExCon(const std::u16string& n)
			:exConNode(creator::createObject(true, this)), name(n) {}
		
		//отключение namedExCon от ноды
		
		virtual ~namedExCon()
		{
			disconnect();
		}
		objectPtr<namedExCon> node() const
		{//внимание!!! Данная функция не может быть вызвана после disconnect!!!
			return objectPtr<namedExCon>(nonTypedObjectPtr(exConNode));
		}
		objectPtr<namedExCon> operator->() const
		{
			return node();
		}
		virtual const ustr& getTypeName() const override
		{
			return name;
		}
		virtual const operation& getOperation(char number) const
		{
			return operation();
		}
	};

	ustr getUstrObjectData(nonTypedObjectPtr objectNode)
	{
		object* obj = obj = objectNode.getObjectPtr();
		if (obj)
		{

			if (objectPtr<namedExCon>::match(objectNode))
				return obj->getTypeName() + dynamic_cast<namedExCon*>(obj)->name;
			return obj->getTypeName();
		}
		else
			return u"nullObjectPtr";
	}
}

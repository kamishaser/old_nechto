#pragma once
#include "object.h"

namespace nechto
{
	class namedExCon :public object
	{
	protected:
		nodePtr exConNode;
		virtual void nodeDisconnect() override
		{
			if(exConNode.exist())
				resetNode(exConNode);
			exConNode = nullptr;
		}
	public:
		const std::wstring name;
		namedExCon(nodePtr emptyObj, const std::wstring& n)
			:name(n) 
		{
			if (objectNullPtr::match(emptyObj))
			{
				objectNullPtr(emptyObj).set(this);
				exConNode = emptyObj;
			}
		}
		namedExCon(const std::wstring& n, bool nodeIsOwner = false)
			:exConNode(creator::createObject(false, this)), name(n) {}
		
		//отключение namedExCon от ноды
		
		virtual ~namedExCon()
		{
			nodeDisconnect();
		}
		objectPtr<namedExCon> node() const
		{//внимание!!! Данная функция не может быть вызвана после disconnect!!!
			return objectPtr<namedExCon>(nonTypedObjectPtr(exConNode));
		}
		objectPtr<namedExCon> operator->() const
		{
			return node();
		}
		virtual const std::wstring& getTypeName() const override
		{
			return name;
		}
		void reconnect(nonTypedObjectPtr node)
		{
			if (!exConNode.exist())
			{
				assert(!node.isUniqueOwner());
				node.set(this);
			}
			else
			{
				node.swapData(nonTypedObjectPtr(exConNode));
			}
			exConNode = node;
		}
	};

	std::wstring getUstrObjectData(nonTypedObjectPtr objectNode)
	{
		object* obj = obj = objectNode.getObjectPtr();
		if (obj)
		{

			if (objectPtr<namedExCon>::match(objectNode))
				return obj->getTypeName() + dynamic_cast<namedExCon*>(obj)->name;
			return obj->getTypeName();
		}
		else
			return L"nullObjectPtr";
	}
	template <class TCon>
	objectPtr<TCon> getObjectPtr(TCon* exCon)
	{
		return objectPtr<TCon>(exCon->node());
	}
}

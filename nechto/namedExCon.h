#pragma once
#include "entity.h"

namespace nechto
{
	//class namedExCon :public entity
	//{
	//protected:
	//	nodePtr exConNode;
	//	virtual void nodeDisconnect() override
	//	{
	//		if(exConNode.exist())
	//			resetNode(exConNode);
	//		exConNode = nullptr;
	//	}
	//public:
	//	const std::wstring name;
	//	namedExCon(nodePtr emptyObj, const std::wstring& n)
	//		:name(n) 
	//	{
	//		if (entityNullPtr::match(emptyObj))
	//		{
	//			entityNullPtr(emptyObj).set(this);
	//			exConNode = emptyObj;
	//		}
	//	}
	//	namedExCon(const std::wstring& n, bool nodeIsOwner = false)
	//		:exConNode(creator::createEntity(false, this)), name(n) {}
	//	
	//	//отключение namedExCon от ноды
	//	
	//	virtual ~namedExCon()
	//	{
	//		nodeDisconnect();
	//	}
	//	entityPtr<namedExCon> node() const
	//	{//внимание!!! Данная функция не может быть вызвана после disconnect!!!
	//		return entityPtr<namedExCon>(nonTypedEntityPtr(exConNode));
	//	}
	//	entityPtr<namedExCon> operator->() const
	//	{
	//		return node();
	//	}
	//	virtual const std::wstring& getTypeName() const override
	//	{
	//		return name;
	//	}
	//	void reconnect(nonTypedEntityPtr node)
	//	{
	//		if (!exConNode.exist())
	//		{
	//			assert(!node.isUniqueOwner());
	//			node.set(this);
	//		}
	//		else
	//		{
	//			node.swapData(nonTypedEntityPtr(exConNode));
	//		}
	//		exConNode = node;
	//	}
	//};

	//std::wstring getUstrEntityData(nonTypedEntityPtr entityNode)
	//{
	//	entity* obj = obj = entityNode.getEntityPtr();
	//	if (obj)
	//	{

	//		if (entityPtr<namedExCon>::match(entityNode))
	//			return obj->getTypeName() + dynamic_cast<namedExCon*>(obj)->name;
	//		return obj->getTypeName();
	//	}
	//	else
	//		return L"nullEntityPtr";
	//}
	//template <class TCon>
	//entityPtr<TCon> getEntityPtr(TCon* exCon)
	//{
	//	return entityPtr<TCon>(exCon->node());
	//}
}

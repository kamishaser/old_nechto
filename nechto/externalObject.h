#pragma once
#include "node.h"
#include "connectionRule.h"

namespace nechto
{
	static const std::wstring unknownTypeName = L"unknownType";
	//объект осуществляющий связь с nechto
	class externalObject
	{
	public:
		virtual const std::wstring& getTypeName() const
		{
			return unknownTypeName;
		}
		virtual const operation& getMethod(char number)const
		{
			return operation();
		}
		virtual const std::wstring getMethodName(char number) const
		{
			return std::wstring();
		}
		virtual const connectionRule& getConnectionRule()const
		{
			return connectionRule::NoneCR;
		}
		nodePtr exObj;

		externalObject(const externalObject&) = delete;

		externalObject(nodePtr conNode)
			:exObj(conNode)
		{
			conNode->setData<externalObject*>(this);
		}
		virtual ~externalObject()
		{
			if (get().exist())
			{
				get()->setData(nullptr);
				deleteNode(get());
			}
		}
		
		const externalObject& operator=(const externalObject&) = delete;
		nodePtr get()const
		{
		return exObj;
		}
		nodePtr getConnection(int number)const
		{
			if (!exObj.exist())
				return nullNodePtr;
			return exObj->connection[number];
		}
		static void initializeNode(nodePtr v1)
		{
			v1->setData(nullptr);
		}
		static void resetNode(nodePtr v1)
		{
			auto data = v1->getData<externalObject*>();
			if (data)
				delete data;
			v1->setData(nullptr);
		}
		///////////////////////////////////////////////////
	private://сей функцианал будет удалён при большом рефакторинге
		
	};
	inline nodePtr newExObjNode(char subtype = 1)
	{
		return newNode(node::ExternalObject, subtype);
	}
#ifdef EXTERNAL_OBJECT_template
	struct EXTERNAL_OBJECT :public externalObject
	{
		EXTERNAL_OBJECT(nodePtr emptyExternalObject)
			:externalObject(emptyExternalObject)
			//при удалении ноды, удалится и сей объект !!!только выделять через new!!!
		{

		}
		virtual ~EXTERNAL_OBJECT()
		{
		}
		/*получение указателя на EXTERNAL_OBJECT по объекту.
		Возвращает nullptr при несоответствии*/
		static EXTERNAL_OBJECT* getByNode(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalObject)
				return nullptr;
			return dynamic_cast<EXTERNAL_OBJECT*>(v1->getData<externalObject*>());
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
	const std::wstring EXTERNAL_OBJECT::typeName = L"nechtoIde.EXTERNAL_OBJECT";
	const connectionRule EXTERNAL_OBJECT::cRule = connectionRule{};
	const staticNodeOperationSet EXTERNAL_OBJECT::methodSet
	{
		namedOperation(L"", operation{
				connectionRule(conRule::ExternalObject, conRule::Input, nullptr,),
				[](nodePtr v0, nodePtr v1, nodePtr v2)
			{
				return true;
			}}),
	};

#endif // 
}
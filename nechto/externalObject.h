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
		virtual nodePtr duplicate()const
		{
			return nullNodePtr; //в данном случае нельзя дублировать
		}
		virtual const connectionRule& getConnectionRule()const
		{
			return connectionRule::NoneCR;
		}
		nodePtr exObj;

		externalObject(const externalObject&) = delete;

		externalObject(nodePtr conNode = nullNodePtr)
		{
			connect(conNode);
		}
		virtual ~externalObject()
		{
			disconnect();
		}
		void newExConNode()
		{
			connect(newNode(node::ExternalObject));
		}
		void deleteExConNode()
		{
			nodePtr temp = exObj;
			exObj = nullNodePtr;
			if (temp.exist())
				deleteNode(temp);
		}
		const externalObject& operator=(const externalObject&) = delete;

		const externalObject& operator=(externalObject&& ec1) noexcept
		{
			nodePtr temp = ec1.exObj;
			ec1.disconnect();
			connect(temp);
			return *this;
		}

		const externalObject& operator=(const nodePtr conNode)
		{
			connect(conNode);
			return *this;
		}
		bool exist() const { return exObj.exist(); }
		nodePtr get()const
		{
			return exObj;
		}
		nodePtr getConnection(int number)const
		{
			nodePtr temp = exObj;
			if (!temp.exist())
				return nullNodePtr;
			return temp->connection[number];
		}
		static void intializeNode(nodePtr v1, externalObject* exObj = nullptr)
		{
			if (exObj)
				exObj->connect(v1);
			else
				v1->setData<externalObject*>(nullptr);
		}
		static void resetNode(nodePtr v1)
		{
			assert(typeCompare(v1, node::ExternalObject));
			auto exObj = v1->getData<externalObject*>();
			if (exObj)
			{
				exObj->disconnect();
				if (v1->getSubtype())//если вледеет данными
					delete exObj;
			}
		}
		///////////////////////////////////////////////////

		void disconnect()
		{
			if (!exObj.exist())
				return;
			exObj->setData<externalObject*>(nullptr);
			exObj = nullNodePtr;
		}
		void connect(nodePtr v1)
		{
			disconnect();
			assert(typeCompare(v1, node::ExternalObject));
			exObj = v1;
			exObj->setData<externalObject*>(this);
		}
		///////////////////////////////////////////////////
		static bool isExCon(nodePtr v1)
		{
			return (v1->getType() == node::ExternalObject);
		}

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
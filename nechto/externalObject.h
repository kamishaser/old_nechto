#pragma once
#include "node.h"
#include "connectionRule.h"

namespace nechto
{
	static const std::wstring unknownTypeName = L"unknownType";
	//объект осуществл€ющий св€зь с nechto
	class externalObject
	{
	public:
		
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
		virtual const std::wstring& getTypeName() const
		{
			return unknownTypeName;
		}
		virtual const operation& getMethod(char number)
		{
			return operation();
		}
		virtual const std::wstring getMethodName(char number)
		{
			return std::wstring();
		}
		virtual nodePtr duplicate()const
		{
			return nullNodePtr; //в данном случае нельз€ дублировать
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
	//externalObject::
}
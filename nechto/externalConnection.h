#pragma once
#include "nodeOperations.h"

namespace nechto
{
	//объект осуществл€ющий св€зь с nechto
	class externalConnection
	{
	public:
		std::atomic<nodePtr> exCon;
		std::wstring typeName;

		externalConnection(const externalConnection&) = delete;

		externalConnection()
		{}
		externalConnection(const std::wstring& n, nodePtr conNode = nullNodePtr)
			:typeName(n)
		{
			connect(conNode);
		}
		virtual ~externalConnection()
		{
			disconnect();
		}
		void newExConNode()
		{
			connect(newNode(node::ExternalConnection));
		}
		void deleteExConNode()
		{
			nodePtr temp = exCon.exchange(nullNodePtr);
			if (temp.exist())
				deleteNode(temp);
		}
		const externalConnection& operator=(const externalConnection&) = delete;

		const externalConnection& operator=(externalConnection&& ec1) noexcept
		{
			nodePtr temp = ec1.exCon;
			ec1.disconnect();
			connect(temp);
			nodeStorage::lock(temp);
			typeName = ec1.typeName;
			nodeStorage::unlock(temp);
			return *this;
		}

		const externalConnection& operator=(const nodePtr conNode)
		{
			connect(conNode);
			return *this;
		}
		bool exist() const { return exCon.load().exist(); }
		auto operator <=> (const externalConnection& ec) const
		{
			return exCon.load() <=> ec.exCon.load();
		}
		nodePtr get()const
		{
			return exCon;
		}
		nodePtr getConnection(int number)const
		{
			nodePtr temp = exCon.load();
			if (!temp.exist())
				return nullNodePtr;
			return temp->connection[number];
		}
		const std::wstring getTypeName() const
		{
			nodeStorage::lock(exCon);
			std::wstring temp = typeName;
			nodeStorage::unlock(exCon);
			return temp;
		}
		void setTypeName(const std::wstring& n)
		{
			nodeStorage::lock(exCon);
			typeName = n;
			nodeStorage::unlock(exCon);
		}

		static void intializeNode(nodePtr v1, externalConnection* exCon = nullptr)
		{
			if (exCon != nullptr)
				exCon->connect(v1);
			else
				v1->setData<externalConnection*>(nullptr);
		}
		static void resetNode(nodePtr v1)
		{
			auto exCon = get(v1);
			if (exCon != nullptr)
				exCon->disconnect();
		}
		///////////////////////////////////////////////////

		void disconnect()
		{
			if (!exCon.load().exist())
				return;
			exCon.load()->setData<externalConnection*>(nullptr);
			exCon = nullNodePtr;
		}
		void connect(nodePtr v1)
		{
			disconnect();
			assert(typeCompare(v1, node::ExternalConnection));
			exCon = v1;
			exCon.load()->setData<externalConnection*>(this);
		}
		///////////////////////////////////////////////////
		static bool isExCon(nodePtr v1)
		{
			return (v1->getType() == node::ExternalConnection);
		}
		static externalConnection* get(nodePtr v1)
		{
			assert(isExCon(v1));
			return v1->getData<externalConnection*>();
		}

	};
}
#pragma once
#include "node.h"
#include "visualNode.h"

namespace nechto::ide
{
	class visualConnection : public externalConnection
	{
		color lineColor{ 255, 255, 255 };
		std::wstring text;
		
		
		visualConnection(visualNode& vn1, visualNode& vn2)
			:externalConnection(newNode(node::ExternalConnection))
		{
			nodePtr v1 = vn1.exCon;
			nodePtr v2 = vn2.exCon;
			NumHubConnect(exCon, v1, 0);
			NumHubConnect(exCon, v2, 1);
		}
		virtual ~visualConnection()
		{
			deleteExConNode();
		}
		static visualConnection* getByPtr(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalConnection)
				return nullptr;
			auto exCon = v1->getData<externalConnection*>();
			if (exCon == nullptr)
				return nullptr;
			if (exCon->getTypeName() != typeName)
				return nullptr;
			return dynamic_cast<visualConnection*>(exCon);
		}
		const static std::wstring typeName;
		virtual const std::wstring& getTypeName() const override
		{
			return typeName;
		}
	};
	const std::wstring visualConnection::typeName = L"nechtoIde.visualConnection";

	struct visualConnectionID
	{
		const nodePtr first;
		const nodePtr second;
		

		bool operator< (const visualConnectionID& c2) const
		{
			if (first < c2.first)
				return true;
			else if (first == c2.first && second < c2.second)
				return true;
			return false;
		}
	};
	struct visualConnectionData
	{
		using oneConNumbers = std::pair<i64, i64>;
		std::vector<oneConNumbers> connections;
	};

	
}
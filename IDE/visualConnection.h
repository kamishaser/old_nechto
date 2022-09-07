#pragma once
#include "node.h"
#include "visualNode.h"
#include "group.h"

namespace nechto::ide
{
	class visualConnection : public externalObject
	{
	public:
		color lineColor{ 255, 255, 255 };
		std::wstring text;
		
		
		visualConnection(nodePtr vConnectionGroup, visualNode* vn1, visualNode* vn2)
			:externalObject(newNode(node::ExternalObject, 1))
			//при удалении ноды, удалится и сей объект. !!!только выделять через new!!!
		{
			assert(vConnectionGroup.exist());
			assert(typeCompare(vConnectionGroup, node::Group));
			IterIterConnect(group::firstEmptyPort(vConnectionGroup), hubIterator(get(), get(), 3));
			nodePtr v1 = vn1->exObj;
			nodePtr v2 = vn2->exObj;
			NumHubConnect(exObj, v1, 0);
			NumHubConnect(exObj, v2, 1);
		}
		virtual ~visualConnection()
		{
			std::cout << "visualConnectionDeleted" << std::endl;
		}
		static visualConnection* getByNode(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalObject)
				return nullptr;
			auto exObj = v1->getData<externalObject*>();
			if (exObj == nullptr)
				return nullptr;
			if (exObj->getTypeName() != typeName)
				return nullptr;
			return dynamic_cast<visualConnection*>(exObj);
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
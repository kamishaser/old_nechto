#pragma once
#include "externalConnection.h"

namespace nechto
{
	class namedExCon : public externalConnection
	{
		std::wstring name;

		namedExCon(nodePtr v1, const std::wstring& n)
			:externalConnection(v1), name(n)
		{
			assert(v1.exist());
			assert(typeCompare(v1, node::ExternalConnection));
		}
		namedExCon(const std::wstring& n)
			:externalConnection(newNode(node::ExternalConnection)),
			name(n) {}
		static namedExCon* getByPtr(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalConnection)
				return nullptr;
			auto exCon = v1->getData<externalConnection*>();
			if (exCon == nullptr)
				return nullptr;
			wchar_t border(L"\'");
			const std::wstring& temp = exCon->getTypeName();
			if (temp.size() < 1)
				return nullptr;
			if (temp.begin() != L"#")
				return nullptr;
			return dynamic_cast<namedExCon*>(exCon);
		}
		virtual const std::wstring& getTypeName() const override
		{
			return std::wstring() + name;
		}
	};
}

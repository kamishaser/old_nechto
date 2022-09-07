#pragma once
#include "externalObject.h"

namespace nechto
{
	class namedExCon : public externalObject
	{
	public:
		std::wstring name;

		namedExCon(nodePtr v1, const std::wstring& n)
			:externalObject(v1), name(n)
		{
			assert(n[0] == L'#');
			assert(v1.exist());
			assert(typeCompare(v1, node::ExternalObject));
		}
		namedExCon(const std::wstring& n)
			:externalObject(newNode(node::ExternalObject)),
			name(n) {assert(n[0] == L'#');}
		static namedExCon* getByNode(nodePtr v1)
		{
			if (!v1.exist())
				return nullptr;
			if (v1->getType() != node::ExternalObject)
				return nullptr;
			auto exObj = v1->getData<externalObject*>();
			if (exObj == nullptr)
				return nullptr;
			const std::wstring& temp = exObj->getTypeName();
			if (temp.size() < 1)
				return nullptr;
			if (temp[0] != L'#')
				return nullptr;
			
			return dynamic_cast<namedExCon*>(exObj);
		}
		virtual const std::wstring& getTypeName() const override
		{
			return name;
		}
		auto operator <=> (const namedExCon& exc) const
		{
			return name <=> exc.name;
		}
	};
}

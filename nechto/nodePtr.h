#pragma once
#include "typeDeclarations.h"
#include "nodeStorage.h"

namespace nechto
{
	
	class nodePtr : public nodeId
	{
	protected:
		auto* node() const
		{
			return nodeStorage::terminal.getNode(*this);
		}
		template <class TCon>
		TCon getData() const //получение данных в формате <TCon>
		{
			assert(sizeof(TCon) <= sizeof(i64));
			i64 temp = node()->data;
			return *static_cast<TCon*>(static_cast<void*>(&temp));
		}
		template <class TCon>
		void setData(TCon Data) //запись данных в формате TCon
		{
			assert(sizeof(TCon) <= sizeof(i64));
			i64 temp = *static_cast<i64*>(static_cast<void*>(&Data));
			node()->data = temp;
		}
		friend class connecter;
		friend class creator;
	public:
		nodePtr(nodeId id)
			:nodeId(id) {}
		nodePtr()
			:nodeId() {}

		nodePtr connection(int number) const
		{
			return node()->port[number];
		}
		char type() const
		{
			return node()->type;
		}

		char subtype() const
		{
			return node()->subtype;
		}

		nodePtr hub() const
		{
			return node()->hubPort;
		}
		
		nodePtr(nullptr_t)
			:nodeId() {}
	};
	class nullNodePtrT : public nodePtr
	{
	public:
		static bool match(const nodePtr& ptr)
		{
			return !ptr.exist();
		}
	};
	template<class TCon>
	concept typedNodePtr =
		requires(TCon node) 
	{
		{node}->std::convertible_to<nodePtr>;
	};
}
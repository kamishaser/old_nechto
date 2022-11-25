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
		TCon getData() const //��������� ������ � ������� <TCon>
		{
			assert(sizeof(TCon) <= sizeof(i64));
			i64 temp = node()->data;
			return *static_cast<TCon*>(static_cast<void*>(&temp));
		}
		template <class TCon>
		void setData(TCon Data) //������ ������ � ������� TCon
		{
			assert(sizeof(TCon) <= sizeof(i64));
			i64 temp = *static_cast<i64*>(static_cast<void*>(&Data));
			node()->data = temp;
		}
		friend class connecter;
		friend class creator;
		friend class hubEraser;
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
	bool typeCompare(nodePtr v1, char type)
	{
		if (!v1.exist())
			return false;
		return (v1.type() == type);
	}
	bool subtypeCompare(nodePtr v1, char subtype)
	{
		if (!v1.exist())
			return false;
		return (v1.subtype() == subtype);
	}
	bool typeSubtypeCompare(nodePtr v1, char type, char subtype)
	{
		if (!v1.exist())
			return false;
		if (v1.type() != type)
			return false;
		if (v1.subtype() != subtype)
			return false;
		return true;
	}
}
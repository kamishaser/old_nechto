#pragma once
#include "typeDeclarations.h"
#include "nodeStorage.h"

namespace nechto
{
	class hubPtr;
	class nodePtr : public nodeId
	{		
		nodePtr(ushort f, ushort s)
			:nodeId(f, s)
		{}
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
		void setData(TCon Data) const //запись данных в формате TCon
		{
			assert(sizeof(TCon) <= sizeof(i64));
			i64 temp = *static_cast<i64*>(static_cast<void*>(&Data));
			node()->data = temp;
		}
		template <class TCon>
		TCon* getDataPtr()
		{
			return node()->getDataPtr<TCon>();
		}
		friend class connecter;
		friend class creator;
		friend class hubManager;
		friend class hubPtr;
		friend class serializer;
		friend class deserializer;
		friend class nodeOperator;
		friend class garbageCollector;
	public:
		nodePtr(nodeId id)
			:nodeId(id) {}
		nodePtr()
			:nodeId() {}

		const nodePtr connection(int number) const
		{
			return node()->port[number];
		}
		unsigned char type() const
		{
			return node()->type;
		}

		unsigned char subtype() const
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
	bool typeCompare(nodePtr v1, unsigned char type)
	{
		if (!v1.exist())
			return false;
		return (v1.type() == type);
	}
	bool subtypeCompare(nodePtr v1, unsigned char subtype)
	{
		if (!v1.exist())
			return false;
		return (v1.subtype() == subtype);
	}
	bool typeSubtypeCompare(nodePtr v1, unsigned char type, unsigned char subtype)
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
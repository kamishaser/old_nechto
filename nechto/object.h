#pragma once
#include "nodePtr.h"
#include "nodeOperationSet.h"
#include "entity.h"
#include <functional>

namespace nechto
{
	class nonTypedEntityPtr : public entityPtr
	{
	protected:
		friend class entity;
		friend class creator;
	public:
		nonTypedEntityPtr(const entityPtr& eptr)
			:entityPtr(eptr)
		{
			assert(match(eptr));
		}
		nonTypedEntityPtr(const existing<nodePtr>& eptr)
			:entityPtr(eptr)
		{
			assert(match(eptr));
		}
		nonTypedEntityPtr(nodePtr eptr)
			:entityPtr(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const entityPtr& eptr)
		{
			return eptr.type() == nodeT::Entity;
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return entityPtr::match(eptr) && (eptr.type() == nodeT::Entity);
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
	};
	using ntObj = nonTypedEntityPtr;


	class entityNullPtr : public nonTypedEntityPtr
	{
	public:
		entityNullPtr(const nonTypedEntityPtr& eptr)
			:nonTypedEntityPtr(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const nonTypedEntityPtr& ptr)
		{
			return !ptr.entityExist();
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return nonTypedEntityPtr::match(eptr) && match(nonTypedEntityPtr(eptr));
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
	};

	

	/*template<class TCon>
	class entityPtr : public nonTypedEntityPtr
	{
	public:
		entityPtr(const nonTypedEntityPtr& eptr)
			:nonTypedEntityPtr(eptr)
		{
			assert(match(eptr));
		}
		entityPtr(existing<nodePtr> eptr)
			:nonTypedEntityPtr(eptr)
		{
			assert(match(eptr));
		}
		entityPtr(nodePtr eptr)
			:nonTypedEntityPtr(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const nonTypedEntityPtr& ptr)
		{
			return dynamic_cast<TCon*>(ptr.getEntityPtr()) != nullptr;
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return nonTypedEntityPtr::match(eptr) && match(nonTypedEntityPtr(eptr));
		}
		static bool match(const nodePtr& ptr)
		{
			return nonTypedEntityPtr::match(ptr) && match(nonTypedEntityPtr(ptr));
		}
		const TCon* operator->() const
		{
			return dynamic_cast<TCon*>(getEntityPtr());
		}
		TCon* operator->()
		{
			return dynamic_cast<TCon*>(getEntityPtr());
		}
		TCon* get()
		{
			return dynamic_cast<TCon*>(getEntityPtr());
		}
	};
	template <class TCon>
	TCon* getEntity(nodePtr node)
	{
		if (!entityPtr<TCon>::match(node))
			return nullptr;
		return entityPtr<TCon>(node).get();
	}*/

#ifdef OBJECT_template
	class OBJECT :public entity
	{
	public:
		const static std::wstring typeName;
		virtual const std::wstring& getName() const
		{
			return typeName;
		}
		virtual const operation& getOperation(char number) const
		{
			return operation();
		}
		virtual void serialize(std::vector<char>& buffer, nonTypedEntityPtr obj) const
		{
			buffer.clear();
		}
	};
	const std::wstring OBJECT::typeName(L"OBJECT");

#endif // 
}
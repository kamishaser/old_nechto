#pragma once
#include "nodePtr.h"
#include "connectionRule.h"
#include "nodeOperationSet.h"
#include "entity.h"
#include <functional>

namespace nechto
{
	class nonTypedObjectPtr : public entityPtr
	{
	protected:
		friend class object;
		friend class creator;
	public:
		nonTypedObjectPtr(const entityPtr& eptr)
			:entityPtr(eptr)
		{
			assert(match(eptr));
		}
		nonTypedObjectPtr(const existing<nodePtr>& eptr)
			:entityPtr(eptr)
		{
			assert(match(eptr));
		}
		nonTypedObjectPtr(nodePtr eptr)
			:entityPtr(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const entityPtr& eptr)
		{
			return eptr.type() == nodeT::Object;
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return entityPtr::match(eptr) && (eptr.type() == nodeT::Object);
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
	};
	using ntObj = nonTypedObjectPtr;


	class objectNullPtr : public nonTypedObjectPtr
	{
	public:
		objectNullPtr(const nonTypedObjectPtr& eptr)
			:nonTypedObjectPtr(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const nonTypedObjectPtr& ptr)
		{
			return !ptr.entityExist();
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return nonTypedObjectPtr::match(eptr) && match(nonTypedObjectPtr(eptr));
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
	};

	

	template<class TCon>
	class objectPtr : public nonTypedObjectPtr
	{
	public:
		objectPtr(const nonTypedObjectPtr& eptr)
			:nonTypedObjectPtr(eptr)
		{
			assert(match(eptr));
		}
		objectPtr(existing<nodePtr> eptr)
			:nonTypedObjectPtr(eptr)
		{
			assert(match(eptr));
		}
		objectPtr(nodePtr eptr)
			:nonTypedObjectPtr(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const nonTypedObjectPtr& ptr)
		{
			return dynamic_cast<TCon*>(ptr.getObjectPtr()) != nullptr;
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return nonTypedObjectPtr::match(eptr) && match(nonTypedObjectPtr(eptr));
		}
		static bool match(const nodePtr& ptr)
		{
			return nonTypedObjectPtr::match(ptr) && match(nonTypedObjectPtr(ptr));
		}
		const TCon* operator->() const
		{
			return dynamic_cast<TCon*>(getObjectPtr());
		}
		TCon* operator->()
		{
			return dynamic_cast<TCon*>(getObjectPtr());
		}
		TCon* get()
		{
			return dynamic_cast<TCon*>(getObjectPtr());
		}
	};
	template <class TCon>
	TCon* getObject(nodePtr node)
	{
		if (!objectPtr<TCon>::match(node))
			return nullptr;
		return objectPtr<TCon>(node).get();
	}

#ifdef OBJECT_template
	class OBJECT :public object
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
		virtual void serialize(std::vector<char>& buffer, nonTypedObjectPtr obj) const
		{
			buffer.clear();
		}
	};
	const std::wstring OBJECT::typeName(L"OBJECT");

#endif // 
}
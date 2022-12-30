#pragma once
#include "nodePtr.h"
#include "connectionRule.h"
#include "nodeOperationSet.h"
#include <functional>

namespace nechto
{
	//объект осуществл€ющий св€зь с nechto
	class object
	{
	public:
		friend class creator;
		virtual ~object() {}
		virtual const std::wstring& getTypeName() const
		{
			return L"nonTypedObject";
		}
		virtual const operation& getMethod(char number) const
		{
			return operation();
		}
		virtual void serialize(std::vector<char>& buffer, existing<nodePtr> obj) const
		{
			buffer.clear();
		}

		//действи€ при отключении от ноды. ќбычно ничего делать не надо
	protected:
		static void resetNode(existing<nodePtr> objectNode);//определено после nonTypedObjectPtr
		virtual void nodeDisconnect() {}
	private:
		void nodeConnect(objectNullPtr object);
	};
	

	class nonTypedObjectPtr : public existing<nodePtr>
	{
	protected:
		
		void setObjectPtr(object* obj)
		{
			setData<object*>(obj);
		}
		friend class object;
		friend class creator;
	public:
		nonTypedObjectPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		nonTypedObjectPtr(nodePtr eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::Object;
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
		bool dataExist() const
		{
			return getObjectPtr() != nullptr;
		}
		//единоличное владение указателем (объектом)
		bool isUniqueOwner() const
		{
			return subtype();
		}
		object* getObjectPtr() const
		{
			return getData<object*>();
		}
		template<class TCon = object>
		TCon* get()
		{
			return dynamic_cast<TCon*>(getObjectPtr());
		}
		void reset()
		{
			if (getObjectPtr() != nullptr)
			{
				if(isUniqueOwner())
					delete getObjectPtr();
				setObjectPtr(nullptr);
			}
		}
		void set(object* object)
		{
			reset();
			setObjectPtr(object);
		}
		object& operator*() const
		{
			return *getObjectPtr();
		}
		object& operator->() const
		{
			return *getObjectPtr();
		}
	};
	using ntObj = nonTypedObjectPtr;
	void object::resetNode(existing<nodePtr> objectNode)
	{
		nonTypedObjectPtr(objectNode).setObjectPtr(nullptr);
	}


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
			return !ptr.dataExist();
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
			return ptr.exist() && match(existing<nodePtr>(ptr));
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
	void object::nodeConnect(objectNullPtr object)
	{
		object.setObjectPtr(this);
	}
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
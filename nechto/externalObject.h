#pragma once
#include "nodePtr.h"
#include "connectionRule.h"
#include "nodeOperationSet.h"

namespace nechto
{
	//объект осуществл€ющий св€зь с nechto
	class externalObject
	{
	public:
		friend class creator;
		struct typeDefinition
		{
			const std::u16string typeName;
			const staticNodeOperationSet operset;
		};
		const static typeDefinition errorType;
		virtual const externalObject::typeDefinition& getType() const
		{
			return errorType;
		}
		virtual ~externalObject() {}
		
		//действи€ при отключении от ноды. ќбычно ничего делать не надо
	protected:
		static void resetNode(existing<nodePtr> objectNode);//определено после nonTypedExternalObjectPtr
		virtual void disconnect() {}
	private:
		void connect(externalObjectNullPtr object);
	};

	template<class TCon>
	concept exTCon = std::is_base_of<externalObject, TCon>::value;

	class nonTypedExternalObjectPtr : public existing<nodePtr>
	{
	protected:
		externalObject* getObjectPtr() const
		{
			return getData<externalObject*>();
		}
		void setObjectPtr(externalObject* obj)
		{
			setData<externalObject*>(obj);
		}
		friend class externalObject;
		friend class creator;
	public:
		nonTypedExternalObjectPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::ExternalObject;
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
		bool owner() const
		{
			return subtype();
		}
		template<exTCon TCon = externalObject>
		TCon* get()
		{
			return dynamic_cast<TCon*>(getObjectPtr());
		}
		void reset()
		{
			if (getObjectPtr() != nullptr)
			{
				if(owner())
					delete getObjectPtr();
				setObjectPtr(nullptr);
			}
		}
		void set(externalObject* object)
		{
			reset();
			setObjectPtr(object);
		}
	};

	void externalObject::resetNode(existing<nodePtr> objectNode)
	{
		nonTypedExternalObjectPtr(objectNode).setObjectPtr(nullptr);
	}


	class externalObjectNullPtr : public nonTypedExternalObjectPtr
	{
	public:
		externalObjectNullPtr(const nonTypedExternalObjectPtr& eptr)
			:nonTypedExternalObjectPtr(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const nonTypedExternalObjectPtr& ptr)
		{
			return !ptr.dataExist();
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return nonTypedExternalObjectPtr::match(eptr) && match(nonTypedExternalObjectPtr(eptr));
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
	};

	

	template<exTCon TCon>
	class externalObjectPtr : public nonTypedExternalObjectPtr
	{
	public:
		externalObjectPtr(const nonTypedExternalObjectPtr& eptr)
			:nonTypedExternalObjectPtr(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const nonTypedExternalObjectPtr& ptr)
		{
			return dynamic_cast<TCon*>(getObjectPtr()) != nullptr;
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return nonTypedExternalObjectPtr::match(eptr) && match(nonTypedExternalObjectPtr(eptr));
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
		const TCon* operator->() const
		{
			return getObjectPtr();
		}
		TCon* operator->()
		{
			return getObjectPtr();
		}
	};

	
	
	const externalObject::typeDefinition externalObject::errorType{u"errorType"};
	void externalObject::connect(externalObjectNullPtr object)
	{
		object.setObjectPtr(this);
	}
#ifdef EXTERNAL_OBJECT_template
	class EXTERNAL_OBJECT :public externalObject
	{
	public:
		const static externalObject::typeDefinition type;
		virtual const externalObject::typeDefinition& getType() const override
		{
			return type;
		}
	};
	const externalObject::typeDefinition EXTERNAL_OBJECT::type
	{u"nechtoIde.EXTERNAL_OBJECT"};

#endif // 
}
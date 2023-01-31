#pragma once
#include "nodePtr.h"
#include "conRuleInterface.h"

namespace nechto
{
	bool isEntityNode(unsigned char type)
	{
		switch (type)
		{
		case nodeT::Entity:
		case nodeT::Operator:
		case nodeT::Text:
			return true;
		default:
			return false;
		}
	}
	template<class TCon>
	concept essT = true;



	//////////////////////////////////////////////////////////////////////////////
	class entityInterface
	{
	protected:
		entityInterface() { ++numberOfEntities; }//сущность существует отдельно
		static void setEntityPtr(existing<nodePtr> esNode, entityInterface* es);
	public:
		/*virtual const operation& getMethod(unsigned char number) const
		{
			return operation();
		}*/
		virtual void serialize(std::vector<char>& buffer, existing<nodePtr> obj) const
		{
			buffer.clear();
		}
		virtual void disconnect(nodePtr esNode)
		{}
		virtual void connect(nodePtr esNode)
		{}
		virtual constexpr const std::wstring& getTypeName() const
		{
			return L"nonTypedEntity";
		}
		virtual ~entityInterface()
		{
			--numberOfEntities;
		}
		static i64 numberOfEntities;
	};
	i64 entityInterface::numberOfEntities = 0;



	/////////////////////////////////////////////////////////////////////entityPtr
	//////////////////////////////////////////////////////////////////////////////
	class entityPtr : public existing<nodePtr>
	{
	protected:
		
		friend class entityInterface;
		friend class creator;
		void setEntityPtr(entityInterface* obj) const
		{
			setData<entityInterface*>(obj);
		}
	public:
		entityPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		entityPtr(const nodePtr& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return isEntityNode(eptr.type());
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
		bool isOneSideLink() const
		{
			return subtype() == entityT::oneSideLink;
		}
		bool entityExist() const
		{
			return getEntityPtr() != nullptr;
		}
		entityInterface* getEntityPtr() const
		{
			return getData<entityInterface*>();
		}
		template<essT ECon>
		ECon* getEntity();//получить конкретного типа сущность
		template<essT ECon>
		ECon* confidentlyGetEntity();//тоже самое, но только с reinterept_cast
		void resetEntity()
		{
			if (entityExist())
			{
				getEntityPtr()->disconnect(*this);
			}
		}
	};
	void entityInterface::setEntityPtr(existing<nodePtr> esNode, entityInterface* es)
	{
		entityPtr(esNode).setEntityPtr(es);
	}



	////////////////////////////////////////////////////////////////////////entity
	//////////////////////////////////////////////////////////////////////////////
	template<essT ECon>
	class entity : public entityInterface
	{
	protected:
		entity(const ECon& d)
			:entityInterface(), data(d) {}
		entity(ECon&& d)
			:entityInterface(), data(std::move(d)) {}

	public:
		ECon data;
		virtual constexpr const std::wstring& getTypeName() const override final
		{
			return typeid(ECon).name();
		}
	};
	

	///////////////////////////////////////////////////////////oneSideLinkedEntity
	//////////////////////////////////////////////////////////////////////////////
	template<essT ECon>
	class oneSideLinkedEntity :public entity<ECon>
	{
	public:
		oneSideLinkedEntity(const ECon& d)
			:entity<ECon>(d) {}
		oneSideLinkedEntity(ECon&& d)
			:entity<ECon>(std::move(d)) {}
		virtual void disconnect(nodePtr enNode) override
		{
			assert(entityPtr::match(enNode));
			assert(enNode.subtype() == entityT::oneSideLink);
			delete this;
		}
		virtual void connect(nodePtr enNode) override
		{
			if (enNode.exist())
			{
				entityInterface::setEntityPtr(enNode, this);
				assert(entityPtr::match(enNode));
				assert(enNode.subtype() == entityT::oneSideLink);
			}
		}
		
	};

	/////////////////////////////////////////////////////////singleConnectedEntity
	//////////////////////////////////////////////////////////////////////////////
	template<essT ECon>
	class singleConnectedEntity :public entity<ECon>
	{
	public:
		singleConnectedEntity(const ECon& d, entityPtr esNode)
			:entity<ECon>(d) { connect(esNode); }
		singleConnectedEntity(ECon&& d, entityPtr esNode)
			:entity<ECon>(std::move(d)) { connect(esNode); }
		singleConnectedEntity(const ECon& d)
			:entity<ECon>(d), enNode(nullptr) {}
		singleConnectedEntity(ECon&& d)
			:entity<ECon>(std::move(d)), enNode(nullptr) {}

		virtual void disconnect(nodePtr entityNode) override
		{
			assert(entityPtr::match(entityNode));
			assert(entityNode == enNode);
			entityInterface::setEntityPtr(entityNode, nullptr);
			enNode = nullptr;
		}
		virtual void connect(nodePtr entityNode) override
		{
			if (enNode.exist())
				entityPtr(enNode).resetEntity();
			enNode = entityNode;
			if (enNode.exist())
			{
				assert(enNode.subtype() == entityT::singleConnection);
				entityPtr(enNode).resetEntity();
				assert(entityPtr::match(enNode));
				entityInterface::setEntityPtr(enNode, this);
			}
		}
		nodePtr node()
		{
			return enNode;
		}
	private:
		nodePtr enNode;
	};
	//////////////////////////////////////////////////////////////////////////////
	template<essT ECon>
	ECon* entityPtr::getEntity()
	{
		auto ptr = dynamic_cast<entity<ECon>*>(getEntityPtr());
		if (ptr == nullptr)
			return nullptr;
		return &ptr->data;
	}
	template<essT ECon>
	ECon* entityPtr::confidentlyGetEntity()
	{
		auto ptr = reinterpret_cast<entity<ECon>*>(getEntityPtr());
		if (ptr == nullptr)
			return nullptr;
		return &ptr->data;
	}
	//////////////////////////////////////////////////////////////////////////////
	//указатель на несуществующую сущность
	class entityNullPtr : public entityPtr
	{
	public:
		entityNullPtr(const entityPtr& eptr)
			:entityPtr(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const entityPtr& ptr)
		{
			return !ptr.entityExist();
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return entityPtr::match(eptr) && match(entityPtr(eptr));
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
	};
	template<essT ECon>
	class typedEntityPtr : public entityPtr
	{
	public:
		typedEntityPtr(const entityPtr& eptr)
			:entityPtr(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const entityPtr& ptr)
		{
			return entityPtr::getEntity<ECon>() != nullptr;
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return entityPtr::match(eptr) && match(entityPtr(eptr));
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
		//получение сущности
		ECon* get()
		{
			return entityPtr::confidentlyGetEntity<ECon>();
		}
	};
	template <essT ECon>
	using teptr = typedEntityPtr<ECon>;

	template <essT ECon>
	ECon* getEntity(teptr<ECon> node)
	{
		return node.confidentlyGetEntity();
	}
}
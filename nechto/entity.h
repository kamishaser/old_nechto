#pragma once
#include "nodePtr.h"
#include "conRuleInterface.h"
#include "enSerInterface.h"

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
		ECon* getEntity() const;//получить конкретного типа сущность
		template<essT ECon>
		ECon* confidentlyGetEntity() const;//тоже самое, но только с reinterept_cast
		void resetEntity();
	};
	//////////////////////////////////////////////////////////////////////////////
	class entityInterface
	{
	protected:
		entityInterface() { ++numberOfEntities; }//сущность существует отдельно
		static void setEntityPtr(existing<nodePtr> esNode, entityInterface* es)
		{
			entityPtr(esNode).setEntityPtr(es);
		}
	public:
		/*virtual const operation& getMethod(unsigned char number) const
		{
			return operation();
		}*/
		virtual void serialize(enSerInteface*, nodePtr) const
		{}
		virtual void eDisconnect(nodePtr esNode)
		{}
		virtual void eConnect(nodePtr esNode)
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

	void entityPtr::resetEntity()
	{
		if (entityExist())
		{
			getEntityPtr()->eDisconnect(*this);
		}
	}

	

	///////////////////////////////////////////////////////////oneSideLinkedEntity
	//////////////////////////////////////////////////////////////////////////////
	template<essT ECon>
	class oneSideLinked :public entityInterface
	{
	public:
		ECon data;
		oneSideLinked(const ECon& d)
			:data(d) {}
		oneSideLinked(ECon&& d)
			:data(std::move(d)) {}
		virtual void eDisconnect(nodePtr enNode) override
		{
			assert(entityPtr::match(enNode));
			assert(enNode.subtype() == entityT::oneSideLink);
			if constexpr (requires(const ECon ec)
			{
				{ec.eDisconnect(entityPtr)};
			})
				data.eDisconnect(enNode);
			delete this;
		}
		virtual void eConnect(nodePtr enNode) override
		{
			if (enNode.exist())
			{
				entityInterface::setEntityPtr(enNode, this);
				assert(entityPtr::match(enNode));
				assert(enNode.subtype() == entityT::oneSideLink);
				if constexpr (requires(const ECon ec)
				{
					{ec.eConnect(entityPtr)};
				})
					data.eConnect(enNode);
			}
		}
		virtual constexpr const std::wstring& getTypeName() const override
		{
			if constexpr (requires(const ECon ec)
			{
				{ec.getTypeName()} -> std::convertible_to<const std::wstring&>;
			})
				return data.getTypeName();
			return L"left noname";// typeid(ECon).name();
		}
		virtual void serialize(enSerInteface* serInterface, nodePtr node) const override
		{
			if constexpr (requires(const ECon ec)
			{
				{ec.serialize(serInterface, node)};
			})
				data.serialize(serInterface, node);
		}
	};
	

	/////////////////////////////////////////////////////////singleConnectedEntity
	//////////////////////////////////////////////////////////////////////////////
	class singleConnectedEntity :public entityInterface
	{
	public:
		singleConnectedEntity(entityPtr esNode)
			{ eConnect(esNode); }
		singleConnectedEntity()
			:enNode(nullptr) {}

		virtual void eDisconnect(nodePtr entityNode) override final
		{
			assert(entityPtr::match(entityNode));
			assert(entityNode == enNode);
			entityInterface::setEntityPtr(entityNode, nullptr);
			enNode = nullptr;
		}
		virtual void eConnect(nodePtr entityNode) override final
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
		nodePtr node() const
		{
			return enNode;
		}
	private:
		nodePtr enNode;
	};
	//////////////////////////////////////////////////////////////////////////////
	template<essT ECon>
	ECon* entityPtr::getEntity() const
	{
		if (isOneSideLink())
		{
			auto ptr = dynamic_cast<oneSideLinked<ECon>*>(getEntityPtr());
			if (ptr == nullptr)
				return nullptr;
			return &ptr->data;
		}
		else
			return dynamic_cast<ECon*>(getEntityPtr());
	}
	template<essT ECon>
	ECon* entityPtr::confidentlyGetEntity() const
	{
		if (isOneSideLink())
		{
			auto ptr = reinterpret_cast<oneSideLinked<ECon>*>(getEntityPtr());
			if (ptr == nullptr)
				return nullptr;
			return &ptr->data;
		}
		else
			return reinterpret_cast<ECon*>( getEntityPtr());
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
			return ptr.getEntity<ECon>() != nullptr;
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
		ECon* get() const
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
	template<essT ECon>
	teptr<ECon> createOneSideLinkedEntity(const ECon& entity)
	{
		return creator::createEntity(entityT::oneSideLink,
			new oneSideLinked<ECon>(entity));
	}
	template<essT ECon, class ... Args>
	teptr<ECon> createOneSideLinkedEntity(entityPtr eptr, Args... arg)
	{
		oneSideLinked<ECon>* ent = new oneSideLinked<ECon>(ECon(eptr, arg...));
		ent->eConnect(eptr);
		return eptr;
	}
}
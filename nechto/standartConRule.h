#pragma once
#include "entity.h"
#include "conRuleInterface.h"
#include "pointerPtr.h"
#include "connectionPointer.h"

namespace nechto
{
	template<elementaryRule elRule>
	struct basicConnectionRule : public conRuleInterface
	{
	protected:
	public:
		static_assert(elRule.type != nodeT::Pointer);
		static_assert(elRule.type != nodeT::Entity);
		static_assert(elRule.type != nodeT::Operator);
		static_assert(elRule.type != nodeT::Text);
		virtual nodePtr getConnection(nodePtr node)const//монструозна€ система получени€ соединени€
		{
			if constexpr (elRule.type == 0)
				return node;
			if (!node.exist())
				return nullptr;
			if constexpr (elRule.transitable)
				if (node.type() == nodeT::Pointer)
				{
					node = pointerPtr(node).follow();
					if (!node.exist())
						return nullptr;
				}
			if (node.type() != elRule.type)
				return nullptr;
			if constexpr (elRule.subtypeImportant)
				if (node.subtype() != elRule.subtype)
					return nullptr;
			return node;
		}
		virtual bool check(nodePtr node) const
		{
			if constexpr (elRule.type == 0)
				return true;
			if (getConnection(node) == nullptr)
				return false;
			return true;
		}
	};
	template<pointerRule ptrRule>
	struct pointerConnectionRule : public conRuleInterface
	{
	protected:
	public:
		virtual nodePtr getConnection(nodePtr node)const//монструозна€ система получени€ соединени€
		{
			if (!node.exist())
				return nullptr;
			if (node.type() != nodeT::Pointer)
				return nullptr;
			if constexpr (ptrRule.subtypeImportant)
				if (node.subtype() != ptrRule.subtype)
					return nullptr;
			if constexpr (ptrRule.necessarilyExistingPointer)
			{
				if (!pointerPtr(node).getHPPair().exist())
					return nullptr;
				if constexpr (ptrRule.ptrContentRule == contentRule::Any)
					return node;
				else if constexpr (ptrRule.ptrContentRule == contentRule::Empty)
				{
					if (pointerPtr(node).getHPPair().follow().exist())
						return nullptr;
					else
						return node;
				}
				else
				{
					if (pointerPtr(node).getHPPair().follow().exist())
						return node;
					else
						return nullptr;
				}
			}
			return node;
		}
		virtual bool check(nodePtr node) const
		{
			if(getConnection(node) == nullptr)
				return false;
			return true;
		}
	};
	template<nonTypedEntityRule entRule>
	struct nonTypedEntityConnectionRule : public conRuleInterface
	{
	protected:
	public:
		virtual nodePtr getConnection(nodePtr node)const//монструозна€ система получени€ соединени€
		{
			if (!node.exist())
				return nullptr;
			if constexpr (entRule.transitable)
				if (node.type() == nodeT::Pointer)
				{
					node = pointerPtr(node).follow();
					if (!node.exist())
						return nullptr;
				}
			if (node.type() != entRule.nodeType)
				return nullptr;
			if constexpr (entRule.subtypeImportant)
				if (node.subtype() != entRule.subtype)
					return nullptr;
			if constexpr (entRule.entityContentRule == contentRule::Any)
				return node;
			else if constexpr (entRule.entityContentRule == contentRule::Empty)
			{
				if (entityPtr(node).entityExist())
					return nullptr;
				else
					return node;
			}
			else
			{
				if (entityPtr(node).entityExist())
					return node;
				else
					return nullptr;
			}
		}
		virtual bool check(nodePtr node) const
		{
			if (getConnection(node) == nullptr)
				return false;
			return true;
		}
	};
	template<entityRule entRule, essT ECon>
	struct entityConnectionRule : public conRuleInterface
	{
	protected:
	public:
		virtual nodePtr getConnection(nodePtr node)const//монструозна€ система получени€ соединени€
		{
			if (!node.exist())
				return nullptr;
			if constexpr (entRule.transitable)
				if (node.type() == nodeT::Pointer)
				{
					node = pointerPtr(node).follow();
					if (!node.exist())
						return nullptr;
				}
			if (node.type() != nodeT::Entity)
				return nullptr;
			if constexpr (entRule.subtypeImportant)
				if (node.subtype() != entRule.subtype)
					return nullptr;
			if (entityPtr(node).getEntity<ECon>() != nullptr)
				return node;
			return nullptr;
		}
		virtual bool check(nodePtr node) const
		{
			if (getConnection(node) == nullptr)
				return false;
			return true;
		}
	};
}

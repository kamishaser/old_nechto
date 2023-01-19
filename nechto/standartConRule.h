#pragma once
#include "entity.h"
#include "conRuleInterface.h"
#include "iteratorPtr.h"
#include "connectionIterator.h"

namespace nechto
{
	template<elementaryRule elRule>
	struct standartOneConnectionRule : public conRuleInterface
	{
	protected:
	public:
		static_assert(elRule.type != nodeT::Iterator);
		static_assert(elRule.type != nodeT::Object);
		static_assert(elRule.type != nodeT::Operator);
		static_assert(elRule.type != nodeT::Text);
		virtual nodePtr getConnection(nodePtr node)const//монструозна€ система получени€ соединени€
		{
			if constexpr (elRule.type == 0)
				return node;
			if (!node.exist())
				return nullptr;
			if constexpr (elRule.transitable)
				if (node.type() == nodeT::Iterator)
				{
					node = iteratorPtr(node).follow();
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
			if (getConnection() == nullptr)
				return false;
			return true;
		}
	};
	template<iteratorRule iterRule>
	struct iteratorConnectionRule
	{
	protected:
	public:
		virtual nodePtr getConnection(nodePtr node)const//монструозна€ система получени€ соединени€
		{
			if (!node.exist())
				return nullptr;
			if (iter.type() != nodeT::Iterator)
				return nullptr;
			if constexpr (iterRule.subtypeImportant)
				if (iter.subtype() != iterRule.subtype)
					return nullptr;
			if constexpr (iterRule.necessarilyExistingIterator)
			{
				if (!iteratorPtr(iter).getHPPair().exist())
					return nullptr;
				if constexpr (iterRule.necessarilyPointToConnectedPort)
					if (!iteratorPtr(iter).follow().exist())
						return nullptr;
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
	struct nonTypedObjectConnectionRule
	{
	protected:
	public:
		virtual nodePtr getConnection(nodePtr node)const//монструозна€ система получени€ соединени€
		{
			if (!node.exist())
				return nullptr;
			if constexpr (entRule.transitable)
				if (node.type() == nodeT::Iterator)
				{
					node = iteratorPtr(node).follow();
					if (!node.exist())
						return nullptr;
				}
			if (node.type() != entRule.nodeType)
				return nullptr;
			if constexpr (entRule.subtypeImportant)
				if (node.subtype() != entRule.subtype)
					return nullptr;
			if constexpr (entRule.necessarilyEmpty)
			{
				if (entityPtr(node).entityExist())
					return nullptr;
				return node;
			}
			return node;
		}
		virtual bool check(nodePtr node) const
		{
			if (getConnection(node) == nullptr)
				return false;
			return true;
		}
	};
	template<entityRule entRule, essT ECon>
	struct objectConnectionRule
	{
	protected:
	public:
		virtual nodePtr getConnection(nodePtr node)const//монструозна€ система получени€ соединени€
		{
			if (!node.exist())
				return nullptr;
			if constexpr (entRule.transitable)
				if (node.type() == nodeT::Iterator)
				{
					node = iteratorPtr(node).follow();
					if (!node.exist())
						return nullptr;
				}
			if (node.type() != nodeT::Object)
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

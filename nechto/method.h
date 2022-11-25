#pragma once
#include "externalObject.h"

namespace nechto
{
	class methodPtr : public existing<nodePtr>
	{
	public:
		static const staticNodeOperationSet operSet;
		methodPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::Method;
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
		const connectionRule defaultConnectionRule =
			connectionRule(conRule::ExternalObject, conRule::useType::readOrWrite);
		nodePtr getObjectNode() const
		{
			return defaultConnectionRule.getConnection(*this, 0);
		}
		template<exTCon TCon>
		TCon* getObject() const
		{
			nonTypedExternalObjectPtr object = getObjectNode();
			if (!object.exist())
				return nullptr;
			return object.get<TCon>();
		}
		const externalObject::typeDefinition* getOjbectType() const
		{
			nodePtr temp = getObjectNode();
			if (!temp.exist())
				return nullptr;
			nonTypedExternalObjectPtr object = existing<nodePtr>(temp);
			if (!object.dataExist())
				return nullptr;
			return &object.get()->getType();
		}
		const operation* getOperation() const
		{
			auto type = getOjbectType();
			if (type == nullptr)
				return nullptr;
			return &type->operset.getOperation(subtype());
		}
		const bool objectExist() const
		{
			return getObjectNode().exist();
		}
	};
}
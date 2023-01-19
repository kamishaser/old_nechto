#pragma once
#include "object.h"

namespace nechto
{
	class methodPtr : public existing<nodePtr>
	{
	public:
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
			connectionRule(conRule::Object, conRule::useType::readOrWrite);
		nodePtr getObjectNode() const
		{
			return defaultConnectionRule.getConnection(*this, 0);
		}
		template<class TCon = object>
		TCon* getObject() const
		{
			nonTypedObjectPtr object = getObjectNode();
			if (!object.exist())
				return nullptr;
			return object.get<TCon>();
		}
		const operation* getMethod() const
		{
			auto obj = getObject();
			if (obj == nullptr)
				return nullptr;
			return &obj->getMethod(subtype());

		}
		const bool objectExist() const
		{
			return getObjectNode().exist();
		
			operationData* opDataPtr()
			{
				return getDataPtr<operationData>();
			}
	};
}
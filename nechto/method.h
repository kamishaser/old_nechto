#pragma once
#include "entity.h"
#include "entity.h"
#include "nodeOperationSet.h"
#include "crSet.h"

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
		/*nodePtr getEntityNode() const
		{
			return defaultConnectionRule.getConnection(*this, 0);
		}
		template<class TCon = entity>
		TCon* getEntity() const
		{
			nonTypedEntityPtr entity = getEntityNode();
			if (!entity.exist())
				return nullptr;
			return entity.get<TCon>();
		}
		const operation* getMethod() const
		{
			auto obj = getEntity();
			if (obj == nullptr)
				return nullptr;
			return &obj->getMethod(subtype());

		}
		const bool entityExist() const
		{
			return getEntityNode().exist();
		}*/
		operationData* opDataPtr()
		{
			return getDataPtr<operationData>();
		}
	};
}
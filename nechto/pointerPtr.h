#pragma once
#include "typeDeclarations.h"
#include "nodePtr.h"
#include "hubPtr.h"
#include "hubPosPair.h"

namespace nechto
{
	class pointerPtr : public existing<nodePtr>
	{
	public:
		explicit pointerPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		explicit pointerPtr(const nodePtr& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::Pointer;
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
		nodePtr follow() const
		{
			if (!exist())
				return nullptr;
			return follow();
		}
		bool checkCorrectness()
		{
			return (purpose().exist()) ? getHPPair().exist() : true;
		}
		ui32 getGlobalPos() const
		{
			return getHPPair().getGlobalPos();
		}
		nodePtr purpose() const
		{
			return connection(1);
		}
		hubPosPair getHPPair() const
		{
			return getData<hubPosPair>();
		}
		void setHPPair(hubPosPair hpp)
		{
			setData(hpp);
		}
	};
	class portPointerPtr : public pointerPtr
	{
	public:
		portPointerPtr(const pointerPtr& eptr)
			:pointerPtr(eptr)
		{
			assert(match(eptr));
		}
		explicit portPointerPtr(const existing<nodePtr>& eptr)
			:pointerPtr(eptr)
		{
			assert(match(eptr));
		}
		explicit portPointerPtr(const nodePtr& eptr)
			:pointerPtr(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const pointerPtr& eptr)
		{
			return !eptr.getHPPair().inGroup();
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return pointerPtr::match(eptr) && match(pointerPtr(eptr));
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}

		bool operator<(const portPointerPtr& ptr)
		{
			assert(purpose() == ptr.purpose());
			return getGlobalPos() < ptr.getGlobalPos();
		}
	}; 
	class groupPointerPtr : public pointerPtr
	{
	public:
		groupPointerPtr(const pointerPtr& eptr)
			:pointerPtr(eptr)
		{
			assert(match(eptr));
		}
		explicit groupPointerPtr(const existing<nodePtr>& eptr)
			:pointerPtr(eptr)
		{
			assert(match(eptr));
		}
		explicit groupPointerPtr(const nodePtr& eptr)
			:pointerPtr(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const pointerPtr& eptr)
		{
			return eptr.getHPPair().inGroup();
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return pointerPtr::match(eptr) && match(pointerPtr(eptr));
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
		bool operator<(const groupPointerPtr& ptr)
		{
			assert(purpose() == ptr.purpose());
			return getGlobalPos() < ptr.getGlobalPos();
		}
	};
}
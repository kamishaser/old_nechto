#pragma once
#include "typeDeclarations.h"
#include "nodePtr.h"
#include "hubPtr.h"
#include "hubPosPair.h"

namespace nechto
{
	class iteratorPtr : public existing<nodePtr>
	{
	public:
		explicit iteratorPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		explicit iteratorPtr(const nodePtr& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::Iterator;
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
		nodePtr follow() const
		{
			auto hpp = getHPPair();
			if (!hpp.exist())
				return nullptr;
			return hpp.follow();
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
	class portIteratorPtr : public iteratorPtr
	{
	public:
		portIteratorPtr(const iteratorPtr& eptr)
			:iteratorPtr(eptr)
		{
			assert(match(eptr));
		}
		explicit portIteratorPtr(const existing<nodePtr>& eptr)
			:iteratorPtr(eptr)
		{
			assert(match(eptr));
		}
		explicit portIteratorPtr(const nodePtr& eptr)
			:iteratorPtr(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const iteratorPtr& eptr)
		{
			return eptr.subtype() == iteratorT::PortIter;
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return iteratorPtr::match(eptr) && match(iteratorPtr(eptr));
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}

		bool operator<(const portIteratorPtr& iter)
		{
			assert(purpose() == iter.purpose());
			return getGlobalPos() < iter.getGlobalPos();
		}
	}; 
	class groupIteratorPtr : public iteratorPtr
	{
	public:
		groupIteratorPtr(const iteratorPtr& eptr)
			:iteratorPtr(eptr)
		{
			assert(match(eptr));
		}
		explicit groupIteratorPtr(const existing<nodePtr>& eptr)
			:iteratorPtr(eptr)
		{
			assert(match(eptr));
		}
		explicit groupIteratorPtr(const nodePtr& eptr)
			:iteratorPtr(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const iteratorPtr& eptr)
		{
			return eptr.subtype() == iteratorT::GroupIter;
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return iteratorPtr::match(eptr) && match(iteratorPtr(eptr));
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
		bool operator<(const groupIteratorPtr& iter)
		{
			assert(purpose() == iter.purpose());
			return getGlobalPos() < iter.getGlobalPos();
		}
	};
}
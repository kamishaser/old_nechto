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
		pointerPtr(const existing<nodePtr>& eptr)
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
		nodePtr follow() const;

		/*������� ��������� ����� �������
		simplePointerPtr, iteratorPtr, conIteratorPtr � groupIteratorPtr */
	};
	class simplePointerPtr : public pointerPtr
	{
	public:
		simplePointerPtr(const pointerPtr& eptr)
			:pointerPtr(eptr)
		{
			assert(match(eptr));
		}
		explicit simplePointerPtr(const existing<nodePtr>& eptr)
			:pointerPtr(eptr)
		{
			assert(match(eptr));
		}
		explicit simplePointerPtr(const nodePtr& eptr)
			:pointerPtr(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const pointerPtr& eptr)
		{
			return !eptr.subtype();
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return pointerPtr::match(eptr) && match(pointerPtr(eptr));
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
		nodePtr follow() const
		{
			return purpose();
		}

		/*������� ��������� ����� �������
		conIteratorPtr � groupIteratorPtr */
	};
	class iteratorPtr : public pointerPtr
	{
	public:
		iteratorPtr(const pointerPtr& eptr)
			:pointerPtr(eptr)
		{
			assert(match(eptr));
		}
		explicit iteratorPtr(const existing<nodePtr>& eptr)
			:pointerPtr(eptr)
		{
			assert(match(eptr));
		}
		explicit iteratorPtr(const nodePtr& eptr)
			:pointerPtr(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const pointerPtr& eptr)
		{
			return eptr.subtype();
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return pointerPtr::match(eptr) && match(pointerPtr(eptr));
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
		static bool match(const pointerPtr& eptr)
		{
			return eptr.subtype() == pointerT::PortIter;
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return pointerPtr::match(eptr) && match(pointerPtr(eptr));
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
		static bool match(const pointerPtr& eptr)
		{
			return eptr.subtype() == pointerT::GroupIter;
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return pointerPtr::match(eptr) && match(pointerPtr(eptr));
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
	nodePtr pointerPtr::follow() const
	{
		if (simplePointerPtr::match(*this))
			return simplePointerPtr(*this).follow();
		else
			return iteratorPtr(*this).follow();
	}
}
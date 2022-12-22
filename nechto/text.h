#pragma once
#include "nodePtr.h"
#include "typeNames.h"
#include <map>


namespace nechto
{
	class textPtr : public existing<nodePtr>
	{
		friend class creator;
		friend class hubManager;
		
	public:
		textPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		textPtr(const nodePtr& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		ustr* getPtr() const
		{
			return getData<ustr*>();
		}
		bool textExist() const
		{
			return getPtr() != nullptr;
		}
		bool owner() const
		{
			return subtype();
		}
		void reset()
		{
			if (textExist())
			{
				if (owner())
					delete getPtr();
				setData<ustr*>(nullptr);
			}
		}
		void set(ustr* text)
		{
			reset();
			setData<ustr*>(text);
		}
		void set(const ustr& text)
		{
			if (textExist())
				*getPtr() = text;
			else
				set(new ustr(text));
		}
		ustr& operator=(const ustr& text)
		{
			set(text);
			return *getPtr();
		}
		i64 getSize() const
		{
			if (!textExist())
				return 0;
			return getPtr()->size();
		}
		ustr& operator*() const
		{
			return *getPtr();
		}
		ustr& operator->() const
		{
			return *getPtr();
		}

		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::Group;
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
	};
}
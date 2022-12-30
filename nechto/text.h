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
		std::wstring* getPtr() const
		{
			return getData<std::wstring*>();
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
				setData<std::wstring*>(nullptr);
			}
		}
		void set(std::wstring* text)
		{
			reset();
			setData<std::wstring*>(text);
		}
		void set(const std::wstring& text)
		{
			if (textExist())
				*getPtr() = text;
			else
				set(new std::wstring(text));
		}
		operator std::wstring& ()
		{
			if (getPtr() == nullptr)
				set(std::wstring());
			return *getPtr();
		}
		std::wstring& operator=(const std::wstring& text)
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

		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::Text;
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
	};
}
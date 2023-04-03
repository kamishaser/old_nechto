#pragma once
#include "nodePtr.h"
namespace nechto
{
	struct portStrenght
	{
		bool port[4];
	};
	struct structData
	{
		portStrenght strenght;
	};
	class structPtr : public existing<nodePtr>
	{
		friend class creator;
		
	public:
		structData getStructData()
		{
			return getData<structData>();
		}
		structData* dataPtr()
		{
			static_assert(sizeof(structData) <= 8);
			return reinterpret_cast<structData*>(&node()->data);
		}
		void setStructData(structData data)
		{
			setData(data);
		}
		structPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		structPtr(const nodePtr& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::Struct;
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
		bool strong(int port)
		{
			return dataPtr()->strenght.port[port];
		}
	};
}
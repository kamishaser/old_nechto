#pragma once
#include "nodePtr.h"
namespace nechto
{
	//хаб каждый хаб обязательно содержит следующие данные:
		//hubConnection: следующий хаб
		//data: предыдущий хаб
	class hubPtr : public existing<nodePtr>
	{
		friend class connecter;
		friend class creator;
		friend class hubManager;

		void connect(existing<nodePtr> previous)
		{
			setData<nodePtr>(previous);
			previous.node()->hubPort = *this;
		}
		void setNext(nodePtr next)
		{
			node()->hubPort = next;
		}
	public:
		
		hubPtr(const existing<nodePtr>& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		hubPtr(const nodePtr& eptr)
			:existing<nodePtr>(eptr)
		{
			assert(match(eptr));
		}
		existing<nodePtr> previous() const
		{
			return existing<nodePtr>(getData<nodePtr>());
		}
		bool empty() const
		{
			if (connection(0).exist() || connection(1).exist() ||
				connection(2).exist() || connection(3).exist())
				return false;
			return true;
		}
		static bool match(const existing<nodePtr>& eptr)
		{
			return eptr.type() == nodeT::Hub;
		}
		static bool match(const nodePtr& ptr)
		{
			return ptr.exist() && match(existing<nodePtr>(ptr));
		}
	};
}
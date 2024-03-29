#pragma once
#include "nodePtr.h"
#include "hubPosPair.h"
namespace nechto
{
	//��� ������ ��� ����������� �������� ��������� ������:
		//hubConnection: ��������� ���
		//data: ���������� ���
	class hubPtr : public existing<nodePtr>
	{
		friend class connecter;
		friend class creator;
		friend class hubManager;
		friend class deserializer;

		void connect(existing<nodePtr> previous, ui32 previousNumber)
		{
			setData<hubPosPair>(hubPosPair(previous, previousNumber + 1));
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
			return existing<nodePtr>(getData<hubPosPair>().hub);
		}
		ui32 number() const
		{
			return getData<hubPosPair>().getGlobalPos();
		}
		bool empty() const
		{
			if (connection(0).exist() || connection(1).exist() ||
				connection(2).exist() || connection(3).exist())
				return false;
			return true;
		}
		bool inGroup() const
		{
			return subtype();
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
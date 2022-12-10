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

		/*функция определна после классов
		simplePointerPtr, iteratorPtr, conIteratorPtr и groupIteratorPtr */
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

		/*функция определна после классов
		conIteratorPtr и groupIteratorPtr */
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
			return eptr.subtype() == pointerT::ConIter;
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
	
	
	namespace pointer
	{
		//bool check(nodePtr v1)
		//{
		//	assert(v1->type() == nodeT::Pointer);
		//	char subtype = v1->getSubtype();
		//	if (subtype == pointer::Simple)
		//		return true;
		//	auto hpp = v1->getData<hubPosPair>();
		//	nodePtr mainNode = v1->connection[0];
		//	nodePtr currentHub = hpp.first;
		//	if (!currentHub.exist() || !mainNode.exist())
		//		return false;
		//	if (currentHub->getData<std::pair<nodePtr, nodePtr>>().second != mainNode)
		//		return false;
		//	if (subtype == pointer::ConIter)
		//	{
		//		if (currentHub != mainNode)//подключено к хабу
		//		{
		//			if (currentHub->getType() != node::Hub)
		//				return false;
		//			if (hub::getMain(currentHub) != mainNode)
		//				return false;
		//		}
		//	}
		//	else
		//	{
		//		if (mainNode->getType() != node::Group)
		//			return false;
		//		if (currentHub->getType() != node::Hub)
		//			return false;
		//		if (hub::getMain(currentHub) != mainNode)
		//			return false;
		//	}
		//	return true;
		//}
		
		
		
		//что хранится в итераторе:
		//в поле данных хранится пара из указателя на хаб и номера позиции в нём
		//к нулевому соединению подключен массив(в случае coniter основная нода)
		
		//присваивание значения ноде того же типа
		//void assigment(nodePtr v0, const nodePtr v1)
		//{
		//	assert(v1.exist());
		//	assert(v1->getType() == node::Pointer);
		//	char subtype = v1->getSubtype();
		//	if (!subtype)//reference
		//	{
		//		NumHubConnect(v0, v1->connection[0], 0);
		//		v0->setData<i64>(0);
		//	}
		//	else
		//	{
		//		NumConnect(v0, v1->connection[0], 0);
		//		NumHubConnect(v0, v1->connection[1], 1);
		//		v0->setData<hubPosPair>(v1->getData<hubPosPair>());
		//	}
		//}
	}

}
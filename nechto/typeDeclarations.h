#pragma once
#include <cassert>
#include <string>
#include <iostream>
#include "enums.h"
namespace nechto
{
	using ushort = unsigned short;
	using i64 = long long;
	using f64 = double;
	using ui32 = unsigned long;

	namespace nodeStorage
	{
		class threadLocalAllocator;
		class nodeAllocator;
	}
	class nodePtr;
	class connecter;
	class creator;
	class hubManager;
	class compressor;
	class nodeOperator;

	struct iterator;
	class portIterator;
	class groupIterator;

	class garbageCollector;

	class serializer;
	class deserializer;

	//базовый класс идентификатора ноды
	struct nodeId
	{
	public:
		ushort first = 0;
		ushort second = 0;

		ushort getFirst() const
		{
			return first;
		}
		ushort getSecond() const
		{
			return second;
		}

		bool operator == (nodeId another) const
		{
			return ((first == another.first) && (second == another.second));
		}
		bool operator != (nodeId another) const
		{
			return !(*this == another);
		}
		bool exist() const
		{
			return first;
		}
		friend class nodeStorage::threadLocalAllocator;
		friend class nodeStorage::nodeAllocator;
		friend class nodePtr;
		friend class garbageCollector;
		friend struct nodeData;

		void operator=(std::nullptr_t)
		{
			first = 0;
			second = 0;
		}
		bool operator==(std::nullptr_t) const
		{
			return !exist();
		}
		bool operator!=(std::nullptr_t) const
		{
			return exist();
		}
		bool operator<(const nodeId id) const
		{
			if (first > id.first)
				return false;
			if (first < id.first)
				return true;
			if (second < id.second)
				return true;
			return false;

		}
	private:
		nodeId(ushort f, ushort s)
			:first(f), second(s) {}
		nodeId() {}
	};
	struct nodeData
	{
		i64 data = 0;
		ushort bottomLevel = 0;
		unsigned char type = 0;
		unsigned char subtype = 0;
		nodeId hubPort;
		nodeId port[4];
		template <class TCon>
		TCon* getDataPtr()
		{
			return static_cast<TCon*>(static_cast<void*>(&Data));
		}
	};
	struct operationData
	{
		bool correctNoTransitConnection[3]{ false, false, false };
		nodeId currentOperator;
	};
	/*operationData* opDataPtr()
		{
			return getDataPtr<operationData>();
		}*/


	template <class TCon>
	class existing : public TCon
	{
	public:
		existing<TCon>(const TCon& tCon)
			:TCon(tCon)
		{
			assert(tCon.exist());
		}
	};
	
	class entityInterface;

	class nullNodePtrT;

	class essencePtr;
	class operationPtr;

	class i64VariablePtr;
	class f64VariablePtr;
	class variablePtr;
	class iteratorPtr;
	class portIteratorPtr;
	class groupIteratorPtr;
	class textPtr;
	class groupPtr;
	class methodPtr;
	class MathOperationPtr;
	class nonTypedObjectPtr;
	class objectNullPtr;

	class namedExCon;
	class namedExConGroup;
	
	bool hasSubType(unsigned char type)
	{
		switch (type)
		{
		case nodeT::Hub:
		case nodeT::Condition:
		case nodeT::Group:
			return false;
		default:
			return true;
		}
	}
	bool hasData(unsigned char type)
	{
		switch (type)
		{
		case nodeT::Variable:
		case nodeT::Text:
		case nodeT::Group:
		case nodeT::Iterator:
			return true;
		default:
			return false;
		}
	}
	bool isOperation(unsigned char type)
	{
		return(type > 127);
	}
	void pConList(nodePtr v1, std::wstring header = std::wstring(L"list of connections"));
}
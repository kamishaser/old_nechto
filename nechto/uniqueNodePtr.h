#pragma once
#include "node.h"
#include "nodeOperations.h"

namespace nechto
{
	class uniqueNodePtr
	{
		nodePtr v = nullNodePtr;
		
	public:
		uniqueNodePtr() {}
		uniqueNodePtr(const uniqueNodePtr&) = delete;
		const uniqueNodePtr& operator=(const uniqueNodePtr&) = delete;

		uniqueNodePtr(uniqueNodePtr&& uv2) noexcept
			:v(uv2.v)
		{
			uv2.del();
		}
		const uniqueNodePtr& operator=(uniqueNodePtr&& uv2) noexcept
		{
			del();
			v = uv2.v;
			uv2.del();
		}
		~uniqueNodePtr()
		{
			del();
		}
		auto operator <=> (const uniqueNodePtr& uNode2) const
		{
			return v <=> uNode2.v;
		}
		void del()
		{
			if (exist())
				deleteNode(v);
		}
		bool exist() const
		{
			return v.exist();
		}
		const nodePtr get() const
		{
			return v;
		}
		operator const nodePtr() const
		{
			return get();
		}
		const node* operator ->() const
		{
			return *v;
		}
		node* operator ->()
		{
			return *v;
		}
		static uniqueNodePtr&& makeUnique(char type = 0, char subtype = 0, size_t data = 0)
		{
			uniqueNodePtr uNodePtr;
			uNodePtr.v = newNode(type, subtype, data);
			return std::move(uNodePtr);
		}
	};
}

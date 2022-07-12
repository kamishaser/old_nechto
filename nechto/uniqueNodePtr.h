#pragma once
#include "node.h"
#include "textOut.h"
#include "nodeOperations.h"

namespace nechto
{
	class uniqueNodePtr
	{
		nodePtr v;
		
	public:
		uniqueNodePtr(char type = 0, char subtype = 0, size_t data = 0)
			:v(newNode(type, subtype, data)) {}
		uniqueNodePtr(const uniqueNodePtr&) = delete;
		const uniqueNodePtr& operator=(const uniqueNodePtr&) = delete;

		uniqueNodePtr(uniqueNodePtr&& uv2) noexcept
			:v(uv2.v)
		{
			std::cout << nodeProperties(v) << std::endl;
			uv2.v = nullNodePtr;
		}
		const uniqueNodePtr& operator=(uniqueNodePtr&& uv2) noexcept
		{
			del();
			v = uv2.v;
			uv2.v = nullNodePtr;
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
			std::cout << "del " << to_string(v) << std::endl;
			if (exist())
				deleteNode(v);
		}
		bool exist() const
		{
			return v.exist();
		}
		nodePtr get() const
		{
			return v;
		}
		operator nodePtr() const
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
	};
}

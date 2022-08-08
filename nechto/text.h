#pragma once
#include "node.h"
#include <map>


namespace nechto
{
	namespace text
	{
		bool isConnectedToNumber(nodePtr v1)
		{
			nodePtr vtemp = v1;
			while (true)
			{
				for (int i1 = 0; i1 < 4; ++i1)
				{
					nodePtr vback = vtemp->connection[i1];
					while (true)
					{
						for (char i = 0; i < 4; i++)
							if (vback->connection[i].load() == v1)
								return true;
						if (!vback->hasHub())
							break;
						vback = vback->hubConnection;
					}
				}
				if (!vtemp->hasHub())
					break;
				vtemp = vtemp->hubConnection;
			}
			return false;
		}
		bool check(nodePtr tag)
		{
			assert(tag.exist());
			assert(tag->getType() == node::Text);
			return true;
		}

		void set(nodePtr v1, const std::wstring& data) noexcept
		{
			assert(v1.exist());
			assert(v1->getType() == node::Text);
			nodeStorage::lock(v1);
			if (v1->getData<std::wstring*>() == nullptr)
				v1->setData(new std::wstring(data));
			else
				*v1->getData<std::wstring*>() = data;
			nodeStorage::unlock(v1);
		}

		std::wstring get(nodePtr v1) noexcept
		{
			assert(v1.exist());
			assert(v1->getType() == node::Text);
			std::wstring temp;
			nodeStorage::lock(v1);
			if (v1->getData<std::wstring*>() != nullptr)
				temp = *v1->getData<std::wstring*>();
			nodeStorage::unlock(v1);
			return temp;
		}
		void reset(nodePtr v1) noexcept
		{
			assert(v1.exist());
			assert(v1->getType() == node::Text);
			nodeStorage::lock(v1);
			if (v1->getData<std::wstring*>() != nullptr)
				delete v1->getData<std::wstring*>();
			v1->setData<std::wstring*>(nullptr);
			nodeStorage::unlock(v1);
		}

		void initialize(nodePtr v1)
		{
			assert(v1.exist());
			assert(v1->getType() == node::Text);
			v1->setData<std::wstring*>(nullptr);
		}
		void initialize(nodePtr v1, const std::wstring& text)
		{
			v1->setData<std::wstring*>(nullptr);
			set(v1, text);
		}
		//присваивание значения ноде того же тип
		void assigment(nodePtr v0, nodePtr v1)
		{
			set(v0, get(v1));
		}
	}
	//std::function<void(nodePtr, size_t)>addTag;
	//std::function<nodePtr(size_t)> GetAddressByID;
	//std::function<>

	
}
#pragma once
#include "node.h"
#include <map>


namespace nechto
{
	namespace text
	{
		static_assert(sizeof(wchar_t) == 2);
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

		void set(nodePtr v1, const std::u16string& data) noexcept
		{
			assert(v1.exist());
			assert(v1->getType() == node::Text);
			nodeStorage::lock(v1);
			if (v1->getData<std::u16string*>() == nullptr)
				v1->setData(new std::u16string(data));
			else
				*v1->getData<std::u16string*>() = data;
			nodeStorage::unlock(v1);
		}

		std::u16string get(nodePtr v1) noexcept
		{
			assert(v1.exist());
			assert(v1->getType() == node::Text);
			std::u16string temp;
			nodeStorage::lock(v1);
			if (v1->getData<std::u16string*>())
				temp = *v1->getData<std::u16string*>();
			nodeStorage::unlock(v1);
			return temp;
		}
		void reset(nodePtr v1) noexcept
		{
			assert(v1.exist());
			assert(v1->getType() == node::Text);
			nodeStorage::lock(v1);
			if (v1->getData<std::u16string*>())
				delete v1->getData<std::u16string*>();
			v1->setData<std::u16string*>(nullptr);
			nodeStorage::unlock(v1);
		}

		void initialize(nodePtr v1)
		{
			assert(v1.exist());
			assert(v1->getType() == node::Text);
			set(v1, u"");
		}
		void initialize(nodePtr v1, const std::u16string& text)
		{
			v1->setData<std::u16string*>(nullptr);
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
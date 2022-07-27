#pragma once
#include "node.h"
#include <map>


namespace nechto
{
	namespace tag
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
			assert(tag->getType() == node::Tag);
			
			if (!tag->hasConnection(0))//наличие соединения)
				return false;
			
			return true;
		}

		static std::mutex tagAdDataBlock[nechto::nodeStorage::maxNumOfAllocators];
		void setData(nodePtr tag, const std::wstring& data) noexcept
		{
			assert(tag.exist());
			assert(tag->getType() == node::Tag);
			tagAdDataBlock[tag.getFirst()].lock();
			if (tag->getData<std::wstring*>() == nullptr)
				tag->setData(new std::wstring(data));
			else
				*tag->getData<std::wstring*>() = data;
			tagAdDataBlock[tag.getFirst()].unlock();
		}

		std::wstring getData(nodePtr tag) noexcept
		{
			assert(tag.exist());
			assert(tag->getType() == node::Tag);
			std::wstring temp;
			tagAdDataBlock[tag.getFirst()].lock();
			if (tag->getData<std::wstring*>() != nullptr)
				temp = *tag->getData<std::wstring*>();
			tagAdDataBlock[tag.getFirst()].unlock();
			return temp;
		}
		void deleteData(nodePtr tag) noexcept
		{
			assert(tag.exist());
			assert(tag->getType() == node::Tag);
			tagAdDataBlock[tag.getFirst()].lock();
			if (tag->getData<std::wstring*>() != nullptr)
				delete tag->getData<std::wstring*>();
			tag->setData<std::wstring*>(nullptr);
			tagAdDataBlock[tag.getFirst()].unlock();
		}

		void initialize(nodePtr v1)
		{
			assert(v1.exist());
			assert(v1->getType() == node::Tag);
			v1->setData<std::wstring*>(nullptr);
		}
		void initialize(nodePtr v1, const std::wstring& text)
		{
			v1->setData<std::wstring*>(nullptr);
			setData(v1, text);
		}
		void reset(nodePtr v1)
		{
			assert(v1.exist());
			assert(v1->getType() == node::Tag);
			deleteData(v1);
		}
		//присваивание значения ноде того же тип
		nodePtr assigment(nodePtr v0, nodePtr v1)
		{
			setData(v0, getData(v1);
		}
	}
	//std::function<void(nodePtr, size_t)>addTag;
	//std::function<nodePtr(size_t)> GetAddressByID;
	//std::function<>

	
}
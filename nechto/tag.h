#pragma once
#include "node.h"
#include <map>

namespace nechto
{
	namespace tag
	{
		enum Type
		{
			Error,
			Name,
			Comment, //стандартный текстовый комментарий
		};
		bool isCorrect(nodePtr tag)
		{
			assert(tag->type == node::Tag);
			
			switch (tag->subtype.load())
			{
			case Comment:
				if (!tag->hasConnection(0))//наличие соединения)
					return false;
				break;
			default:
				return false;
			}
			return true;
		}

		class nodeAddDataMapPart
		{//часть словаря nodeData с нодами зранящимися в определённом аллокаторе
			std::mutex mapBlock;
			std::map<ushort, std::string> adData;

		public:
			void set(ushort address, std::string temp) noexcept
			{
				mapBlock.lock();
				if (adData.contains(address))
					adData.at(address) = std::move(temp);
				else
					adData.emplace(address, std::move(temp));
				mapBlock.unlock();
			}
			std::string&& get(ushort address) noexcept
			{
				std::string temp;
				mapBlock.lock();
				if (adData.contains(address))
					temp = adData.at(address);
				mapBlock.unlock();
				return std::move(temp);
			}
			void erase(ushort address) noexcept
			{
				mapBlock.lock();
				auto i = adData.find(address);
				if (i != adData.end())
					adData.erase(address);
				mapBlock.unlock();
			}
		};
		static nodeAddDataMapPart nodeAddDataMap[nodeStorage::maxNumOfAllocators];

		void setData(nodePtr address, std::string& data) noexcept
		{
			nodeAddDataMap[address.getFirst()].set(address.getSecond(), data);
		}
		std::string&& getData(nodePtr address) noexcept
		{
			return nodeAddDataMap[address.getFirst()].get(address.getSecond());
		}
		void eraseAdData(nodePtr address) noexcept
		{
			nodeAddDataMap[address.getFirst()].erase(address.getSecond());
		}
	}
	//std::function<void(nodePtr, size_t)>addTag;
	//std::function<nodePtr(size_t)> GetAddressByID;
	//std::function<>
}
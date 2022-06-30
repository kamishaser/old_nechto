#pragma once
#include "node.h"
#include <map>


namespace nechto
{
	namespace tag
	{
		//вспомогательная информация
		
		enum Type
		{
			Error,
			Comment, //стандартный текстовый комментарий
			Name,

			ExternalConnection, //внешнее подключение. Данная нода может использоваться извне.
			//ноду имеющую внешнее подключение нельзя удалять.
			//комонента связности, не имеющаа внешнего подключение считается утеренной и 
			//может быть удалена сборщиком мусора, (когда он будет разработан)
			Attribute
		};
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
		bool isCorrect(nodePtr tag)
		{
			assert(tag.exist());
			assert(tag->getType() == node::Tag);
			
			if (!tag->hasConnection(0))//наличие соединения)
				return false;
			
			return true;
		}

		static std::mutex tagAdDataBlock[nechto::nodeStorage::maxNumOfAllocators];
		void setData(nodePtr tag, const std::string& data) noexcept
		{
			assert(tag.exist());
			assert(tag->getType() == node::Tag);
			tagAdDataBlock[tag.getFirst()].lock();
			std::cout << tag->getData<std::string*>() << std::endl;
			if (tag->getData<std::string*>() == nullptr)
				tag->setData(new std::string(data));
			else
				*tag->getData<std::string*>() = data;
			tagAdDataBlock[tag.getFirst()].unlock();
		}

		std::string getData(nodePtr tag) noexcept
		{
			assert(tag.exist());
			assert(tag->getType() == node::Tag);
			std::string temp;
			tagAdDataBlock[tag.getFirst()].lock();
			if (tag->getData<std::string*>() != nullptr)
				temp = *tag->getData<std::string*>();
			tagAdDataBlock[tag.getFirst()].unlock();
			return temp;
		}
		void deleteData(nodePtr tag) noexcept
		{
			assert(tag.exist());
			assert(tag->getType() == node::Tag);
			tagAdDataBlock[tag.getFirst()].lock();
			if (tag->getData<std::string*>() != nullptr)
				delete tag->getData<std::string*>();
			tag->setData<std::string*>(nullptr);
			tagAdDataBlock[tag.getFirst()].unlock();
		}
		auto operator <=> (const nodePtr& tag1, const nodePtr& tag2)
		{
			assert(tag1.exist());
			assert(tag1->getType() == node::Tag);
			assert(tag2.exist());
			assert(tag2->getType() == node::Tag);
			return tag::getData(tag1) <=> tag::getData(tag2);
		}
	}
	//std::function<void(nodePtr, size_t)>addTag;
	//std::function<nodePtr(size_t)> GetAddressByID;
	//std::function<>
}
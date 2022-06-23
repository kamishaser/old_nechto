#pragma once
#include "node.h"
#include <map>

namespace nechto
{
	namespace tag
	{
		//таг - вспомогательная, не являющаяся частью исполняемого алгоритма информация
		enum Type
		{
			Error,
			Comment, //стандартный текстовый комментарий
			Name,

			ExternalConnection, //внешнее подключение. Данная нода может использоваться извне.
			//ноду имеющую внешнее подключение нельзя удалять.
			//комонента связности, не имеющаа внешнего подключение считается утеренной и 
			//может быть удалена сборщиком мусора, (когда он будет разработан)
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


		void setData(nodePtr tag, std::string& data) noexcept
		{
			std::cout << tag->getData<std::string*>() << std::endl;
			if (tag->getData<std::string*>() == nullptr)
				tag->setData(new std::string(data));
			else
				*tag->getData<std::string*>() = data;
		}
		std::string getData(nodePtr tag) noexcept
		{
			if (tag->getData<std::string*>() == nullptr)
				return std::string();
			else
				return *tag->getData<std::string*>();
		}
		void deleteData(nodePtr tag) noexcept
		{
			if (tag->getData<std::string*>() != nullptr)
				delete tag->getData<std::string*>();
			tag->setData<std::string*>(nullptr);
		}
	}
	//std::function<void(nodePtr, size_t)>addTag;
	//std::function<nodePtr(size_t)> GetAddressByID;
	//std::function<>
	/*class externalConnection
	{
		nodePtr exConTag;
	public:
		externalConnection(nodePtr conNode)
		{
			if (conNode->type == node::Tag && conNode->subtype == tag::ExternalConnection)
				exConTag = conNode;
		}
	};*/
}
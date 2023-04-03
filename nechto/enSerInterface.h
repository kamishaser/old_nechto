#pragma once
#include "nodePtr.h"
#include <deque>

namespace nechto
{
	//интерфейс сериализации сущности
	class enSerInteface
	{
	public:
		virtual void write(const char*, i64 size) {}
		virtual ~enSerInteface() {}
	};
	//интерфейс десериализации сущности
	class enDeserInterface
	{
	public:
		virtual void read(char*, i64 size) {}
		virtual const std::wstring& getTypeName()
		{
			return std::wstring();
		}
		virtual nodePtr getNode()
		{
			return nullptr;
		}
		virtual ~enDeserInterface() {}
	};
	//сериализация сущности в буффер
	class bufferSerInterface : public enSerInteface
	{
	public:
		std::vector<char> buffer;

		virtual void write(const char* ch, i64 size)
		{
			size_t oldSize = buffer.size();
			buffer.resize(oldSize + size);
			for (int i = 0; i < size; ++i)
			{
				buffer[oldSize + i] = ch[i];
			}				
		}
	};
	class bufferDeserInterface : public enDeserInterface
	{
	public:
		std::deque<char> buffer;
		std::wstring typeName;
		nodePtr enNode;

		virtual void read(char* ch, i64 size)
		{
			assert(size <= buffer.size());
			size_t oldSize = buffer.size();
			buffer.resize(oldSize + size);
			for (int i = 0; i < size; ++i)
			{
				ch[i] = buffer.front();
				buffer.pop_front();
			}
		}
		virtual const std::wstring& getTypeName()
		{
			return typeName;
		}
		virtual nodePtr getNode()
		{
			return enNode;
		}
	};
}
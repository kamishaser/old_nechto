#pragma once
#include "serializer.h"
#include <filesystem>
#include <fstream>

namespace nechto
{
	class fileSerializer : protected serializer
	{
		std::ofstream stream;
	public:
		fileSerializer()
			:serializer([&](const char* p, uint32_t size)
				{stream.write(p, size); }) {}
		void open(std::filesystem::path path)
		{
			assert(!isOpen());
			stream.open(path, std::ios::binary | std::ios::trunc);
			ui32 type = 0;
			ui32 version = 0;
			stream.write(reinterpret_cast<const char*>(&type), 4);
			stream.write(reinterpret_cast<const char*>(&version), 4);
		}
		bool isOpen()
		{
			return stream.is_open();
		}
		void close()
		{
			assert(isOpen());
			stream.close();
		}
		void serialize(nodePtr v1)
		{
			serializer::serialize(v1);
		}
	};
	class fileDeserializer : protected deserializer
	{
		std::ifstream stream;
	public:
		fileDeserializer(deserializer::enDeserType enDeserFunc)
			:deserializer([&](char* p, uint32_t size)
				{stream.read(p, size); }, enDeserFunc) {}
		void open(std::filesystem::path path)
		{
			assert(!isOpen());
			stream.open(path, std::ios::binary);
			ui32 type = 0;
			ui32 version = 0;
			stream.read(reinterpret_cast<char*>(&type), 4);
			stream.read(reinterpret_cast<char*>(&version), 4);
			assert(type == 0);
			assert(version == 0);
		}
		bool isOpen()
		{
			return stream.is_open();
		}
		void close()
		{
			assert(isOpen());
			stream.close();
		}
		nodePtr deserialize()
		{
			return deserializer::deserialize();
		}
	};
}

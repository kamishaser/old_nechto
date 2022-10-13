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
			
		}
		bool isOpen()
		{
			return stream.is_open();
		}
		void close()
		{
			assert(isOpen());
			end();
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
		fileDeserializer()
			:deserializer([&](char* p, uint32_t size)
				{stream.read(p, size); }) {}
		void open(std::filesystem::path path)
		{
			assert(!isOpen());
			stream.open(path, std::ios::binary);

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

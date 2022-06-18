#pragma once
#include "stream.h"
#include <fstream>
#include <filesystem>

namespace nechto
{
	class fileStream
	{
		stream			nechto_string;
		std::fstream	string_file;
	public:
		explicit fileStream(std::function<std::string(nodePtr)>	getAdditionalNodeData = nullptr,
			std::function<void(nodePtr, std::string)>	setAdditionalNodeData = nullptr)
		{
			nechto_string.getAdditionalNodeData = getAdditionalNodeData;
			nechto_string.setAdditionalNodeData = setAdditionalNodeData;
			nechto_string.read = [&](char* c, int32_t s) {string_file.read(c, s); };
			nechto_string.write = [&](const char* c, int32_t s) {string_file.write(c, s); };
		}
		bool sOpen(std::filesystem::path path)
		{
			string_file.open(path, std::ios::out | std::ios::binary);
			return string_file.is_open();
		}
		bool isOpen()
		{
			return string_file.is_open();
		}
		bool isSaved(const nodePtr v1)
		{
			return nechto_string.isSaved(v1);
		}
		void save(const nodePtr v1)
		{
			assert(isOpen());
			nechto_string.save(v1);
		}
		void close()
		{
			nechto_string.saveEnd();
			string_file.close();
		}
		bool load(std::filesystem::path path)
		{
			if (isOpen())
				return false;
			string_file.open(path, std::ios::in | std::ios::binary);
			if (!isOpen())
				return false;
			nechto_string.load();
			string_file.close();
			return true;
		}
	};
}
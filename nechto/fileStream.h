#pragma once
#include "stream.h"
#include <fstream>
#include <filesystem>

namespace nechto
{
	/*class fileStream
	{
		stream			nechtoStream;
		std::fstream	fStream;
		
	public:
		explicit fileStream()
			:nechtoStream([&](const char* c, int32_t s) {fStream.write(c, s); },
					[&](char* c, int32_t s) {fStream.read(c, s); }){}

		bool saveStart(std::filesystem::path path)
		{
			assert(!isOpen());
			fStream.open(path, std::ios::out | std::ios::binary);
			if (!fStream.is_open())
				return false;
			nechtoStream.saveStart();
			return true;
		}
		bool loadStart(std::filesystem::path path)
		{
			assert(!isOpen());
			fStream.open(path, std::ios::in  | std::ios::binary);
			if (!fStream.is_open())
				return false;
			nechtoStream.loadStart();
			return true;
		}
		bool isOpen()
		{
			return fStream.is_open();
		}
		bool isSaved(const nodePtr v1)
		{
			return nechtoStream.isSaved(v1);
		}
		void save(const nodePtr v1)
		{
			assert(isOpen());
			nechtoStream.save(v1);
		}
		void end()
		{
			nechtoStream.end();
			fStream.close();
		}
		nodePtr load()
		{
			assert(isOpen());
			return nechtoStream.load();
		}
	};*/
}
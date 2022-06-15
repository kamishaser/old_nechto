#pragma once
#include "node.h"
#include <filesystem>
#include <fstream>
#include <map>
#include <set>

namespace nechto
{
	class nechtoOutFileStream
	{
	private:
		std::set<nodePtr> savedNodes;
		std::ofstream out;
	public:
		nechtoOutFileStream();
		bool open(std::filesystem::path path)
		{
			out.open(path, std::ios::binary | std::ios::trunc | std::ios::out);
			return out.is_open();
		}
		void close()
		{
			savedNodes.clear();
			out.close();
		}
		void clear()
		{
			savedNodes.clear();
			out.clear();
		}
		nechtoOutFileStream& operator<<(nodePtr v1)
		{
			savedNodes.insert(v1);
			
			auto type    = v1->type.load();
			auto subtype = v1->subtype.load();
			auto data    = v1->data.load();

			out.write(reinterpret_cast<char*>(&v1), sizeof(v1));
			out.write(reinterpret_cast<char*>(&type), sizeof(type));
			out.write(reinterpret_cast<char*>(&subtype), sizeof(subtype));
			out.write(reinterpret_cast<char*>(&data), sizeof(data));

			for (int i = 0; i < 3; i++)
			{
				nodePtr temp = 
					(savedNodes.contains(v1->connection[i].load())) ? v1->connection[i]
					:nullNodePtr;
				out.write(reinterpret_cast<char*>(&temp), sizeof(temp));
			}
			return *this;
		}
		
	};
}

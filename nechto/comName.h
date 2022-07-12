#pragma once
#include "node.h"
#include <string>
#include <vector>
#include <compare>
namespace nechto
{
	class comName
	{
		std::vector <std::string> nameComponents;
	public:
		comName(const std::string& name, char separator = '.')
		{
			int64_t lastPosition = 0;
			for (int64_t i = 0; true; ++i)
			{
				if (name[i] == separator)
				{
					nameComponents.push_back(name.substr(lastPosition,
						i - lastPosition));
					lastPosition = i + 1;
				}
				if (i == name.size())
				{
					nameComponents.push_back(name.substr(lastPosition,
						i - lastPosition));
					break;
				}
			}
		}
		int64_t nameSize() const
		{
			int64_t size = 0;
			for (auto i = nameComponents.begin(); i != nameComponents.end(); ++i)
				size += i->size();
			size += nameComponents.size() - 1;//separators
			return size;
		}
		int64_t comNum() const
		{
			return static_cast <int64_t> (nameComponents.size());
		}
		const std::string str(char separator = '.') const
		{
			std::string name;
			name.reserve(nameSize());
			for (int64_t i = 0; true; i++)
			{
				name += nameComponents[i];
				if (i + 1 >= static_cast<int64_t>(nameComponents.size()))
					break;
				name.push_back(separator);
			}
			return name;
		}
		operator const std::string() const 
		{
			return str();
		}
		const std::string& getComponent(int64_t number = 0) const
		{
			return nameComponents[number];
		}
		std::strong_ordering operator<=>(const comName&) const = default;
		bool isIn(const comName& directory)const
		{
			if (nameComponents.size() <= directory.nameComponents.size())
				return false;
			for (int64_t i = 0; i < directory.comNum(); ++i)
			{
				if (nameComponents[i] != directory.nameComponents[i])
					return false;
			}
			return true;
		}
		const comName& operator +=(const comName& topName)
		{
			for (auto i = topName.nameComponents.begin(); i != topName.nameComponents.end(); i++)
				nameComponents.push_back(*i);
			return *this;
		}
		const comName operator+(const comName& topName)
		{
			comName result(*this);
			result += topName;
			return result;
		}
		int matchesAtBegin(comName& cn)
		{
			int max = (nameComponents.size() < cn.nameComponents.size()) ?
				nameComponents.size() : cn.nameComponents.size();
			int i = 0;
			for (; i < max; ++i)
				if (nameComponents[i] != cn.nameComponents[i])
					return i;
			return max;
		}
		int matchesAtEnd(comName& cn)
		{
			int max = ((nameComponents.size() < cn.nameComponents.size()) ?
				nameComponents.size() : cn.nameComponents.size());
			int i = 0;
			for(; i < max; ++i)
				if (nameComponents[max-i-1] != cn.nameComponents[max-i-1])
					return i;
			return max;
		}

	};
	
	comName operator "" _cmn(const char* str, size_t size)
	{
		return comName(std::string(str, size));
	}
}

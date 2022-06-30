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
		comName(const std::string& name)
		{
			int64_t lastPosition = 0;
			for (int64_t i = 0; true; ++i)
			{
				if (name[i] == '.')
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
		const std::string str() const
		{
			std::string name;
			name.reserve(nameSize());
			for (int64_t i = 0; true; i++)
			{
				name += nameComponents[i];
				if (i + 1 >= static_cast<int64_t>(nameComponents.size()))
					break;
				name.push_back('.');
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
		static bool isGlobalAndLocal(const comName& n1, const comName& n2)
		{
			const comName* global;
			const comName* local;
			if (n1.comNum() > n2.comNum())
			{
				global = &n1;
				local = &n2;
			}
			else
			{
				global = &n2;
				local = &n1;
			}
			const int64_t offset = global->comNum() - local->comNum();
			for (int64_t i = 0; i < local->comNum(); ++i)
			{
				if (local->getComponent(i) != global->getComponent(i + offset))
					return false;
			}
			return true;
		}
	};
	
}

#pragma once
#include <map>

#include "visual.h"
#include "comName.h"
#include "uniqueNodePtr.h"

namespace nechto::ide
{
	//class visualAtribute
	//{
	//	std::map<comName, uniqueNodePtr> attributes;
	//public:
	//	enum Type
	//	{
	//		Error,
	//		Int64,
	//		Double,
	//		String
	//	};

	//	/*void add(const comName& n)
	//	{
	//		attributes.emplace(n, std::move(uniqueNodePtr(node::Variable)));
	//	}
	//	void add(const comName& n, double startValue)
	//	{
	//		auto i = attributes.emplace(n,
	//			std::move(uniqueNodePtr(node::Variable, baseValueType::Double))).first;
	//		i->second.get()->setData<double>(startValue);
	//	}
	//	void add(const comName& n, int64_t startValue)
	//	{
	//		auto i = attributes.emplace(n,
	//			std::move(uniqueNodePtr(node::Variable, baseValueType::Int64))).first;
	//		i->second.get()->setData<double>(startValue);
	//	}
	//	void add(const comName& n, std::string startValue)
	//	{
	//		auto i = attributes.emplace(n,
	//			std::move(uniqueNodePtr(node::Tag, tag::Comment))).first;
	//		tag::setData(i->second.get(), startValue);
	//	}*/

	//	bool contains(const comName& n)
	//	{
	//		return attributes.contains(n);
	//	}
	//	Type type(const comName& n)
	//	{
	//		assert(contains(n));
	//		nodePtr v1 = attributes.at(n).get();
	//		if (v1->getType() == node::Variable)
	//		{
	//			if (v1->getSubtype() == baseValueType::Int64)
	//				return Type::Int64;
	//			if (v1->getSubtype() == baseValueType::Double)
	//				return Type::Double;
	//		}
	//		if (v1->getType() == node::Tag)
	//			return Type::String;
	//		return Error;
	//	}
	//	int64_t getIntData(const comName& n)
	//	{
	//		assert(contains(n));
	//		assert(type(n) == Int64);
	//		return attributes.at(n).get()->getData<int64_t>();
	//	}
	//	double getDoubleData(const comName& n)
	//	{
	//		assert(contains(n));
	//		assert(type(n) == Double);
	//		return attributes.at(n).get()->getData<double>();
	//	}
	//	std::string getStringData(const comName& n)
	//	{
	//		assert(contains(n));
	//		assert(type(n) == String);
	//		return tag::getData(attributes.at(n).get());
	//	}
	//	void erase(const comName& n)
	//	{
	//		assert(contains(n));
	//		attributes.erase(n);
	//	}
	//};
}
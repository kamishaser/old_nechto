#pragma once
#include <map>

#include "visual.h"

namespace nechto::ide
{
	//class visualAtribute
	//{
	//	std::map<comName, uniqueNodePtr> attributes;
	//public:
	//	enum Type
	//	{
	//		Error,
	//		I64,
	//		F64,
	//		String
	//	};

	//	/*void add(const comName& n)
	//	{
	//		attributes.emplace(n, std::move(uniqueNodePtr(node::Variable)));
	//	}
	//	void add(const comName& n, f64 startValue)
	//	{
	//		auto i = attributes.emplace(n,
	//			std::move(uniqueNodePtr(node::Variable, baseValueType::F64))).first;
	//		i->second.get()->setData<f64>(startValue);
	//	}
	//	void add(const comName& n, i64 startValue)
	//	{
	//		auto i = attributes.emplace(n,
	//			std::move(uniqueNodePtr(node::Variable, baseValueType::I64))).first;
	//		i->second.get()->setData<f64>(startValue);
	//	}
	//	void add(const comName& n, std::wstring startValue)
	//	{
	//		auto i = attributes.emplace(n,
	//			std::move(uniqueNodePtr(node::Text, tag::Comment))).first;
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
	//			if (v1->getSubtype() == baseValueType::I64)
	//				return Type::I64;
	//			if (v1->getSubtype() == baseValueType::F64)
	//				return Type::F64;
	//		}
	//		if (v1->getType() == node::Text)
	//			return Type::String;
	//		return Error;
	//	}
	//	i64 getIntData(const comName& n)
	//	{
	//		assert(contains(n));
	//		assert(type(n) == I64);
	//		return attributes.at(n).get()->getData<i64>();
	//	}
	//	f64 getF64Data(const comName& n)
	//	{
	//		assert(contains(n));
	//		assert(type(n) == F64);
	//		return attributes.at(n).get()->getData<f64>();
	//	}
	//	std::wstring getStringData(const comName& n)
	//	{
	//		assert(contains(n));
	//		assert(type(n) == String);
	//		return text::get(attributes.at(n).get());
	//	}
	//	void erase(const comName& n)
	//	{
	//		assert(contains(n));
	//		attributes.erase(n);
	//	}
	//};
}
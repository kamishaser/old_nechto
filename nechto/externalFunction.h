#pragma once
#include "node.h"
#include "baseValueTypes.h"

#include <functional>
#include <set>

namespace nechto
{
	class externalFunction
	{
	private:
		
	public:
		const std::string name;
		const std::function<bool(nodePtr)> isCorrect;
		const nodeEvent Func = nullptr;

		externalFunction(std::string fname, std::function<bool(nodePtr)> check = [](nodePtr c) {return false; }, nodeEvent Function = nullptr)
			:name(fname),isCorrect(check), Func(Function)
		{}


		bool operator==(const externalFunction& fun2)const
		{return name == fun2.name;}
		bool operator!=(const externalFunction& fun2)const
		{return name != fun2.name;}
		bool operator< (const externalFunction& fun2)const
		{return name < fun2.name;}
		bool operator> (const externalFunction& fun2)const
		{return name > fun2.name;}
		bool operator<=(const externalFunction& fun2)const
		{return name <= fun2.name;}
		bool operator>=(const externalFunction& fun2)const
		{return name >= fun2.name;}
	};

	static std::set<externalFunction> funSet{externalFunction("error")};
	static std::mutex setBlock;

	bool addExternalFunction(const externalFunction newFun) noexcept
	{
		setBlock.lock();
		if (funSet.contains(newFun))
		{//проверяется только по имени
			setBlock.unlock();
			return false;
		}
		funSet.emplace(newFun);
		setBlock.unlock();
		return true;
	}
	bool isExternalFunctionExist(const std::string& name) noexcept
	{
		const externalFunction temp(name, nullptr, nullptr);//сиЕсть большой костыль
		setBlock.lock();
		bool result = funSet.contains(temp);//проверяется только по имени
		setBlock.unlock();
		return result;
	}
	const externalFunction* getExternalFunction(const std::string& name) noexcept
	{
		const externalFunction temp(name, nullptr, nullptr);//сиЕсть большой костыль
		setBlock.lock();
		const externalFunction* function = (funSet.contains(temp)) ? &(*funSet.find(temp)) : nullptr;
		//проверяется только по имени
		setBlock.unlock();
		return function;
	}
}

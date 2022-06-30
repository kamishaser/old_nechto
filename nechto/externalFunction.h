#pragma once
#include "node.h"
#include "baseValueTypes.h"
#include "comName.h"

#include <functional>
#include <set>
//Внимание! Приготовьтесь! Вас ждёт страна костылей!!!

namespace nechto
{
	class externalFunction
	{
	private:
		static std::mutex setBlock;
		static std::set<externalFunction> funSet;

	public:
		comName name;
		mutable std::atomic<bool(*)(nodePtr)> isCorrectPtr;
		mutable std::atomic<void(*)(nodePtr)> FuncPtr;

		externalFunction(comName fname, bool(*check)(nodePtr), void(*Function)(nodePtr))
			:name(fname),isCorrectPtr(check), FuncPtr(Function)
		{}
		externalFunction(const externalFunction& f)
			:name(f.name), isCorrectPtr(f.isCorrectPtr.load()), FuncPtr(f.FuncPtr.load()) {}
		const externalFunction& operator=(const externalFunction& f)
		{
			name = f.name;
			isCorrectPtr = f.isCorrectPtr.load();
			FuncPtr = f.FuncPtr.load();
		}

		bool isCorrect(nodePtr v1)
		{
			return isCorrectPtr.load()(v1);
		}
		void perform(nodePtr v1)
		{
			FuncPtr.load()(v1);
		}
		auto operator <=> (const externalFunction& exCon)const
		{
			return name <=> exCon.name;
		}

		static const externalFunction Error;


		static const externalFunction add(const externalFunction newFun) noexcept
		{
			setBlock.lock();
			if (funSet.contains(newFun))
			{//проверяется только по имени
				auto ExtFun = funSet.find(newFun);//стрёмная конструкция(проверка только по имени)
				const externalFunction oldFun(newFun.name, ExtFun->isCorrectPtr.load(), ExtFun->FuncPtr.load());
				ExtFun->isCorrectPtr = newFun.isCorrectPtr.load();
				ExtFun->FuncPtr = newFun.FuncPtr.load();
				setBlock.unlock();
				return oldFun;
			}
			funSet.emplace(newFun);
			setBlock.unlock();
			return Error;
		}
		static bool exist(const std::string& name) noexcept
		{
			const externalFunction temp(name, nullptr, nullptr);//сиЕсть большой костыль
			setBlock.lock();
			bool result = funSet.contains(temp);//проверяется только по имени
			setBlock.unlock();
			return result;
		}
		static const externalFunction* get(const std::string& name) noexcept
		{
			const externalFunction temp(name, nullptr, nullptr);//сиЕсть большой костыль
			setBlock.lock();
			const externalFunction* function = (funSet.contains(temp)) ? &(*funSet.find(temp)) : nullptr;
			//проверяется только по имени
			setBlock.unlock();
			return function;
		}
	};
	const externalFunction externalFunction::Error =
		externalFunction(std::string("error"), [](nodePtr c) {return false; }, [](nodePtr) {});
	std::mutex externalFunction::setBlock;
	std::set<externalFunction> externalFunction::funSet;
	
	

	
}

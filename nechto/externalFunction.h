#pragma once
#include "node.h"

#include <functional>
#include <set>

//внешняя функция, подключаемая в нечто из с++
namespace nechto::externalFunction
{
	//структура, описывающая саму функцию
	struct fn
	{
		//проверка корректности подключения
		std::atomic<bool(*)(nodePtr)> correctCheckPtr;
		std::atomic<bool(*)(nodePtr)> fnPtr;
		//комменария, описание действия
		std::wstring description;
		
		//список нод, использующих эту функцию
		std::atomic<i64> numberOfUsers = 0;

		fn(bool(*t)(nodePtr), bool(*f)(nodePtr), const std::wstring& d)
			:correctCheckPtr(t), fnPtr(f), description(d) {}
		fn(const fn& f)
			:correctCheckPtr(f.correctCheckPtr.load()), fnPtr(f.fnPtr.load()),
			description(f.description), numberOfUsers(0) {}

		const fn& operator= (const fn& f)
		{
			correctCheckPtr = f.correctCheckPtr.load();
			fnPtr = f.fnPtr.load();
			description = f.description;
		}
		//выполнить
		bool perform(nodePtr v1)
		{
			return (fnPtr.load())(v1);
		}
		//проверить
		bool check(nodePtr v1)
		{
			return (correctCheckPtr.load())(v1);
		}
	};
	//в качестве данных нода содержит 
	//указатель (реализован итератором) на функцию
	struct exFun
	{//блое имя	/ функция для хранения в set
		mutable std::shared_ptr<fn> function;
		std::wstring name;

		exFun(const std::wstring& n, std::shared_ptr<fn> fun)
			:name(n), function(fun) {}

		auto operator <=> (const exFun& exF) const
		{
			return name <=> exF.name;
		}
	};

	//к сожалению попытка запихнуть имя и функцию в одну структуру привела 
	//к жесточайшим костылям. Такую структуру ни в map, ни в set без костылей
	//типа mutable не запихнёшь

	const std::shared_ptr<fn> Error = std::make_shared<fn>
		(
			[](nodePtr) {return false; },
			[](nodePtr) {return false; },
			L"error: undefined function"

		);
	//map используемых функций
	struct externalFunctionSet
	{
		std::set<exFun> funset;
		std::mutex mblock;

		externalFunctionSet()
		{
			insert
			(
				L"Error",
				Error
			);
		}
		bool contains(const std::wstring& name) noexcept
		{
			mblock.lock();
			bool temp = funset.contains(exFun(name, Error));
			mblock.unlock();
			return temp;
		}
		const exFun* insert(const std::wstring& name, const std::shared_ptr<fn> fn) noexcept
		{
			mblock.lock();
			const exFun* temp = &(*funset.emplace(exFun(name, fn)).first);
			mblock.unlock();
			return temp;
		}
		const exFun* find(const std::wstring& name) noexcept
		{
			assert(contains(name));
			mblock.lock();
			const exFun* temp = &(*funset.find(exFun(name, Error)));
			mblock.unlock();
			return temp;
		}
		bool erase(const std::wstring& name) noexcept
		{
			if (!contains(name))
				return false;
			
			mblock.lock();
			bool result = !funset.find(exFun(name, Error))->function->numberOfUsers;
			if(result)
				funset.erase(exFun(name, Error));
			mblock.unlock();
			return result;
		}
	};
	using shEFS = std::shared_ptr<externalFunctionSet>;
	//учтанавливает функцию в ноду
	void set(nodePtr v1, shEFS funSet, const std::wstring name) noexcept
	{
		const exFun* temp = funSet->contains(name) ?
			funSet->find(name) :
			funSet->insert
			(
				name,
				Error
			);
		--v1->getData<const exFun*>()->function->numberOfUsers;
		v1->setData<const exFun*>(temp);
		++temp->function->numberOfUsers;

	}
	//заполняет ноду функции
	void initializeEmpty(nodePtr v1)
	{	
		v1->setData<const exFun*>(nullptr);
	}
	void initialize(nodePtr v1, shEFS& funMap, const std::wstring name = L"Error")
	{
		const exFun* temp = funMap->contains(name) ?
			funMap->find(name) :
			funMap->insert
			(
				name,
				Error
			);
		v1->setData<const exFun*>(temp);
		++temp->function->numberOfUsers;
	}
	//сбрасывает функцию
	void reset(nodePtr v1)
	{
		const exFun* temp = v1->getData<exFun*>();
		if(temp != nullptr)
			--temp->function->numberOfUsers;
	}
	bool perform(nodePtr v1)
	{
		return v1->getData<exFun*>()->function->perform(v1);
	}
	bool check(nodePtr v1)
	{
		return (v1->getData<exFun*>()->function->check(v1));
	}
	//присваивание значения ноде того же типа
	void assigment(nodePtr v0, nodePtr v1)
	{
		const exFun* temp = v1->getData<exFun*>();
		--v0->getData<const exFun*>()->function->numberOfUsers;
		v0->setData<const exFun*>(temp);
		++temp->function->numberOfUsers;
	}
	std::wstring getName(nodePtr v1)
	{
		if (!v1.exist())
			return L"error";
		if (!typeCompare(v1, node::ExternalFunction))
			return L"error";
		return v1->getData<const exFun*>()->name;
	}
}

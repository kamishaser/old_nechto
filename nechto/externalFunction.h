#pragma once
#include "node.h"

#include <functional>
#include <set>

//������� �������, ������������ � ����� �� �++
namespace nechto::externalFunction
{
	//���������, ����������� ���� �������
	struct fn
	{
		//�������� ������������ �����������
		std::atomic<bool(*)(nodePtr)> correctCheckPtr;
		std::atomic<void(*)(nodePtr)> fnPtr;
		//����������, �������� ��������
		std::wstring comment;
		
		//������ ���, ������������ ��� �������
		std::atomic<i64> numberOfUsers = 0;

		fn(bool(*t)(nodePtr), void(*f)(nodePtr), const std::wstring& c)
			:correctCheckPtr(t), fnPtr(f), comment(c) {}
		//���������
		void perform(nodePtr v1)
		{
			(fnPtr.load())(v1);
		}
		//���������
		bool check(nodePtr v1)
		{
			bool (correctCheckPtr.load())(v1);
		}
	};
	//� �������� ������ ���� �������� 
	//��������� (���������� ����������) �� �������
	using exFun = std::map<std::wstring, fn>::iterator;

	//� ��������� ������� ��������� ��� � ������� � ���� ��������� ������� 
	//� ����������� ��������. ����� ��������� �� � map, �� � set ��� ��������
	//���� mutable �� ��������

	static_assert(sizeof(exFun) == 8);

	fn Error =
		fn
		(
			[](nodePtr) {return false; },
			[](nodePtr) {},
			L"error: undefined function"

		);
	//map ������������ �������
	struct map
	{
		std::map<std::wstring, fn> funset;
		std::mutex mblock;

		map()
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
			bool temp = funset.contains(name);
			mblock.unlock();
			return temp;
		}
		exFun insert(const std::wstring& name, const fn& fn) noexcept
		{
			mblock.lock();
			exFun temp = funset.emplace(name, fn).first;
			mblock.unlock();
			return temp;
		}
		exFun find(const std::wstring& name) noexcept
		{
			assert(contains(name));
			mblock.lock();
			exFun temp = funset.find(name);
			mblock.unlock();
			return temp;
		}
		bool erase(const std::wstring& name) noexcept
		{
			if (!contains(name))
				return false;
			
			mblock.lock();
			bool result = funset.at(name).isNotUsed();
			if(result)
				funset.erase(name);
			mblock.unlock();
			return result;
		}
	};
	using shmap = std::shared_ptr<map>;
	//������������� ������� � ����
	void set(nodePtr v1, shmap funMap, const std::wstring name) noexcept
	{
		exFun temp = funMap->contains(name) ?
			funMap->find(name) :
			funMap->insert
			(
				name,
				Error
			);
		--v1->getData<exFun>()->second.numberOfUsers;
		v1->setData<exFun>(temp);
		++temp->second.numberOfUsers;

	}
	//��������� ���� �������
	void initialize(nodePtr v1, shmap funMap, const std::wstring name = L"Error")
	{
		exFun temp = funMap->contains(name) ?
			funMap->find(name) :
			funMap->insert
			(
				name,
				Error
			);
		v1->setData<exFun>(temp);
		++temp->second.numberOfUsers;
	}
	//���������� �������
	void reset(nodePtr v1)
	{
		exFun temp = v1->getData<exFun>();
		--temp->second.numberOfUsers;
	}
	void perform(nodePtr v1)
	{
		v1->getData<exFun>()->second.perform();
	}
	bool check(nodePtr v1)
	{
		return (v1->getData<exFun>()->second.perform();
	}
	//������������ �������� ���� ���� �� ����
	void assigment(nodePtr v0, nodePtr v1)
	{
		exFun temp = v1->getData<exFun>();
		--v0->getData<exFun>()->second.numberOfUsers;
		v0->setData<exFun>(temp);
		++temp->second.numberOfUsers;
	}
}

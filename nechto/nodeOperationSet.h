#pragma once
#include <array>
#include "conRuleInterface.h"

namespace nechto
{
	template<int size = 256>
	class nodeOperationSet
	{
	public:
		std::array<operation, size> opSet;
		const unsigned char usedSize;
		//массив всех операций

		//инициализация по initializer_list
		nodeOperationSet(
			std::initializer_list<operation> ilist)
			:usedSize(static_cast<char>(ilist.size()))
		{
			assert(ilist.size() < 256);
			int number = 0;
			for (auto& i : ilist)
			{
				opSet[number] = i;
				number++;
			}
		}
		nodeOperationSet()
			:usedSize(0) {}
		//инициализация копированием с добавлением
		nodeOperationSet(
			const nodeOperationSet<size>& nopSet,
			std::initializer_list<operation> ilist)
			:usedSize(nopSet.usedSize + ilist.size())
		{
			assert((ilist.size() + nopSet.usedSize) < 256);
			int number = 0;
			for (; number < nopSet.usedSize; ++number)
			{
				opSet[number] = nopSet.opSet[number];
			}
			for (auto& i : ilist)
			{
				opSet[number] = i;
				number++;
			}
		}
		constexpr char getNumber(const std::wstring& name) const
		{
			for (int i = 0; i < usedSize; ++i)
				if (nameSet[i] == name)
					return i;
		}
		const operation* getOperation(unsigned char number) const
		{
			return &opSet[number];
		}

		bool operate(nodePtr v1, unsigned char number) const
		{
			return opSet[number].operate(v1);
		}

	};
	
}


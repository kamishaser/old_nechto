#pragma once
#include "connectionRule.h"
#include <array>

namespace nechto
{
	using namedOperation = std::pair<std::wstring, operation>;
	class staticNodeOperationSet
	{
	public:
		std::array<operation, 256> opSet;
		std::array<std::wstring, 256> nameSet;
		unsigned char size;
		//������ ���� ��������

		//������������� �� initializer_list
		staticNodeOperationSet(
			std::initializer_list<namedOperation> ilist)
			:size(static_cast<char>(ilist.size()))
		{
			assert(ilist.size() < 256);
			char number = 0;
			for (auto& i : ilist)
			{
				nameSet[number] = i.first;
				opSet[number] = i.second;
				number++;
			}
		}
		staticNodeOperationSet()
			:size(0) {}
		//������������� ������������ � �����������
		staticNodeOperationSet(
			const staticNodeOperationSet& snoSet,
			std::initializer_list<namedOperation> ilist)
			:size(snoSet.size + ilist.size())
		{
			assert((ilist.size() + snoSet.size) < 256);
			unsigned char number = 0;
			for (; number < snoSet.size; ++number)
			{
				nameSet[number] = snoSet.nameSet[number];
				opSet[number] = snoSet.opSet[number];
			}
			for (auto& i : ilist)
			{
				nameSet[number] = i.first;
				opSet[number] = i.second;
				number++;
			}
		}

		constexpr const std::wstring& getName(unsigned char number) const
		{
			return nameSet[number];
		}
		constexpr char getNumber(const std::wstring& name) const
		{
			for (int i = 0; i < 256; ++i)
				if (nameSet[i] == name)
					return i;
		}
		const operation& getOperation(unsigned char number) const
		{
			return opSet[number];
		}

		bool operate(nodePtr v1, unsigned char number) const
		{
			return opSet[number].operate(v1);
		}

	};
	
}


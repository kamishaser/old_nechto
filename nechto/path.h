#pragma once
#include<cassert>
#include "pointerPtr.h"

namespace nechto
{
	using pathData = std::array<uchar, 8>;
	class path
	{
		pathData transitionSequence{ 0,0,0,0,0,0,0,0 };
		ui32 size = 0;
		//mutable std::array<nodePtr, 8> stepData{ nullptr };
	public:
	
		enum transitionType
		{
			n0, //переход по номеру
			n1,
			n2,
			n3,
			portPointerTransition, //переход по указателю
			pathTransition, //переход по ноде пути
			intermediaryConnection, //посредник подключения
			//проверка соотвествия типу entity и существования сущности
			existingEntity, 
		};
		path() {}
		path(const char* str, size_t Size)
			:size(Size)
		{
			if (size > 8)
				size = 8;
			for (int i = 0; i < size; ++i)
			{
				transitionSequence[i] = typeByChar(str[i]);
			}
		}

		
		constexpr int getSize() const
		{
			return size;
		}
		constexpr bool pushBack(transitionType type)
		{
			if (size >= 8)
				return false;
			transitionSequence[size] = type;
			++size;
			return true;
		}
		constexpr void popBack()
		{
			if (size == 0)
				return;
			--size;
		}
		constexpr bool pushFront(transitionType trType)
		{
			if (size >= 8)
				return false;
			for (int i = 7; i > 0; --i)
			{
				transitionSequence[i] = transitionSequence[i - 1];
			}
			transitionSequence[0] = trType;
			--size;
			return true;
		}
		constexpr void popFront()
		{
			if (size == 0)
				return;
			for (int i = 0; i < size; ++i)
			{
				transitionSequence[i] = transitionSequence[i + 1];
			}
		}
		bool operator==(path p2) const
		{
			if (size != p2.size)
				return false;
			for (int i = 0; i < size; ++i)
				if (transitionSequence[i] != p2.transitionSequence[i])
					return false;
			return true;
		}
		bool operator!=(path p2) const
		{
			if (size != p2.size)
				return true;
			for (int i = 0; i < 8; ++i)
				if (transitionSequence[i] != p2.transitionSequence[i])
					return true;
			return false;
		}
		bool operator<(path p2) const
		{
			if (size < p2.size)
			{
				for (ui32 i = 0; i < size; ++i)
				{
					if (transitionSequence[i] > p2.transitionSequence[i])
						return false;
					else if (transitionSequence[i] < p2.transitionSequence[i])
						return true;
				}
				return true;
			}
			else
			{
				for (ui32 i = 0; i < p2.size; ++i)
				{
					if (transitionSequence[i] > p2.transitionSequence[i])
						return false;
					else if (transitionSequence[i] < p2.transitionSequence[i])
						return true;
				}
				return false;
			}
			
		}
		bool startWith(path p2) const
		{
			for (ui32 i = 0; i < p2.size; ++i)
				if (transitionSequence[i] != p2.transitionSequence[i])
					return false;
			return true;
		}
		path operator+(path p2) const
		{
			path temp = *this;
			assert(size + p2.size <= 8);
			for (ui32 i = 0; i < p2.size; ++i)
				temp.transitionSequence[size + i] = p2.transitionSequence[i];
			temp.size = size + p2.size;
			return temp;
		}
		path operator/(path p2) const //тоже самое, что и +
		{
			return operator+(p2);
		}
		path operator-()const
		{
			path temp = *this;
			if (temp.size > 0)
				--temp.size;
			return temp;
		}
		constexpr uchar last() const
		{
			assert(size > 0);
			return transitionSequence[size - 1];
		}
		nodePtr operator/(nodePtr node) const
		{
			return follow(node);
		}
		const path& operator+=(path p2)
		{
			assert(size + p2.size <= 8);
			for (ui32 i = 0; i < p2.size; ++i)
				transitionSequence[size + i] = p2.transitionSequence[i];
			size += p2.size;
			return *this;
		}
		static constexpr transitionType typeByChar(char ch)
		{
			switch (ch)
			{
			case '0':
				return n0;
			case '1':
				return n1;
			case '2':
				return n2;
			case '3':
				return n3;
			case 'i':
				return portPointerTransition;
			case 'p':
				return pathTransition;
			case 'c':
				return intermediaryConnection;
			case 'e':
				return existingEntity;
			
			default:
				assert(false);
			}
		}
		nodePtr step(int number, existing<nodePtr> node, transitionType trType) const
		{
			switch (trType)
			{
			case portPointerTransition:
				//if (!pointerPtr::match(node))
					//return stepData[number] = nullptr;
				//return stepData[number] = pointerPtr(node).follow();

			case pathTransition:
				assert(false); //недоделано
				break;
			case intermediaryConnection:
				assert(false);
				break;
			case existingEntity:
				if (entityPtr::match(node))
					return nullptr;
				if (!entityPtr(node).entityExist())
					return nullptr;
				return node;
			case path::n0:
				return node.connection(0);
			case path::n1:
				return node.connection(1);
			case path::n2:
				return node.connection(2);
			case path::n3:
				return node.connection(3);
			default:
				return nullptr;
			}
		}
		nodePtr follow(nodePtr node) const
		{
			for (int i = 0; i < size; ++i)
			{
				if (!node.exist())
					return nullptr;
				node = step(i, node,
					static_cast<transitionType>(transitionSequence[i]));
			}
			return node;
		}
	};
	path operator "" _np(const char* str, size_t size)
	{
		return path(str, size);
	}
}
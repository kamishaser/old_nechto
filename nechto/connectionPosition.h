#pragma once
namespace nechto
{
	class connectionPosition
	{
		uint32_t position;
		const static uint32_t grK = 0x80000000ui32;
		//константа для определения, состоит ли cp в группе
		const static uint32_t poK = 0x7fffffffui32;
		//константа для определения глобальной позиции
	public:
		connectionPosition(uint32_t pos, bool inGroup)
			:position((inGroup) ? (pos | inGroup) : pos)
		{}

		bool inGroup() const
		{
			return position & grK;
		}
		char getLocalPos() const
		{
			return position & 3ui32;
		}
		uint32_t getGlobalPos() const
		{
			return position & poK;
		}
		void setLocalPos(uint32_t pos)
		{
			position = position - getLocalPos() + pos;
		}
		//Установить глобальную позицию
		void setGlobalPos(uint32_t pos)
		{
			position = pos | (position & grK);
		}
		void setGroup(bool value)
		{
			position = (value) ?
				position | value :
				position & poK;
		}

		//получить номер хаба
		uint32_t getHubNumber() const
		{
			return (position & poK) >> 2;
		}
		//установить номер хаба
		void setHubNumber(uint32_t number)
		{
			setGlobalPos((number << 2) + getLocalPos());
		}
		void operator+= (uint32_t number)
		{
			position += number;
		}
		void operator-= (uint32_t number)
		{
			position -= number;
		}
		bool operator++()
		{
			return ((++position) & 3ui32) == 0;
		}
		bool operator--()
		{
			return ((--position) & 3ui32) == 3;
		}

	};
	static_assert(sizeof(connectionPosition) == 4);
}
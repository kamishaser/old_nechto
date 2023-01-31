#pragma once
namespace nechto
{
	class connectionPosition
	{
		uint32_t position;
		const static uint32_t grK = 0x80000000ui32;
		//��������� ��� �����������, ������� �� cp � ������
		const static uint32_t poK = 0x7fffffffui32;
		//��������� ��� ����������� ���������� �������
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
		//���������� ���������� �������
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

		//�������� ����� ����
		uint32_t getHubNumber() const
		{
			return (position & poK) >> 2;
		}
		//���������� ����� ����
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
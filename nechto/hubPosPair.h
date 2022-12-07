#pragma once
#include "nodePtr.h"

namespace nechto
{
	struct hubPosPair
	{
	private:
		uint32_t position;
	public:
		nodePtr hub;

		hubPosPair(nodePtr h = nullptr, uint32_t p = 0)
			:hub(h), position(p) {}

		char getLocalPos() const
		{
			return position & 3ui32;
		}
		uint32_t getGlobalPos() const
		{
			return position;
		}
		void setLocalPos(char pos)
		{
			position = position - getLocalPos() + pos;
		}
		//Опасно!!! Установить глобальную позицию
		void setGlobalPos(uint32_t pos)
		{
			position = pos;
		}
		//прямая ссылка на позицию
		uint32_t& posRef()
		{
			return position;
		}
		bool operator++()
		{
			if (((++position) & 3U) == 0)
				return true;
			return false;
		}
		bool operator--()
		{
			if (((--position) & 3U) == 3)
				return true;
			return false;
		}


		bool exist() const
		{
			return hub.exist();
		}
		nodePtr follow() const
		{
			return hub.connection(getLocalPos());
		}
		bool operator== (const hubPosPair hpp) const
		{
			return (hub == hpp.hub) && (position == hpp.position);
		}
		bool operator!= (const hubPosPair hpp) const
		{
			return (hub != hpp.hub) || (position != hpp.position);
		}
	};
}
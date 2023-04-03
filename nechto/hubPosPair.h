#pragma once
#include "nodePtr.h"
#include "reverseConnectionPosition.h"

namespace nechto
{
	struct hubPosPair
	{
	private:
		uint32_t position;
		friend struct pointer;
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
		//Установить глобальную позицию
		void setGlobalPos(uint32_t pos)
		{
			position = pos;
		}
		//получить номер хаба
		ui32 getHubNumber() const
		{
			return getGlobalPos() >> 2;
		}
		//установить номер хаба
		void setHubNumber(uint32_t number)
		{
			position = (number << 2) + getLocalPos();
		}
		//прямая ссылка на позицию
		uint32_t& posRef()
		{
			return position;
		}
		bool exist() const
		{
			return hub.exist();
		}
		nodePtr follow() const
		{
			if (!hub.exist())
				return nullptr;
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
		bool inGroup() const
		{
			return (hub.type() == nodeT::Hub) && (hub.subtype() == 1);
		}
	protected:
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
		void shift(uint32_t offset)
		{
			position += offset;
		}
	};
}
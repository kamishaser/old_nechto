#pragma once
#include <cassert>
namespace nechto
{
	class reverseConnectionPosition
	{
		unsigned char conPos = 0;

	public:
		int getLocalPos(unsigned port) const
		{
			assert(port < 4);
			return (conPos & mask(port)) >> pos(port);
		}
		void setLocalPos(unsigned port, unsigned reversePort)
		{
			assert(port < 4);
			conPos = ((conPos & (~mask(port))) | (reversePort << pos(port)));
		}
		void clear()
		{
			conPos = 0;
		}
	private:
		inline static constexpr unsigned char mask(unsigned char port)
		{
			assert(port < 4);
			return ((unsigned char)3) << pos(port);
		}
		inline static constexpr unsigned char pos(unsigned char port)
		{
			return (port << 1);
		}
	};
	static_assert(sizeof(reverseConnectionPosition) == 1);
}
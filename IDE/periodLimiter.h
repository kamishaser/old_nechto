#pragma once
#include <chrono>

namespace nechto::ide
{
	using namespace std::chrono;

	milliseconds currentTime()
	{
		return duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	}
	class periodLimiter
	{
	public:
		milliseconds min;
		milliseconds max;//если максимум меньше минимума, максимум не используется
		milliseconds lastUp;

		periodLimiter(milliseconds minPeriod, milliseconds maxPeriod = 0ms)
			:min(minPeriod), max(maxPeriod), lastUp(currentTime()) {}

		void reset()
		{
			lastUp = currentTime();
		}
		milliseconds currentPeriod()
		{
			milliseconds timeInterval = currentTime() - lastUp;
			if ((timeInterval > max) && (max > min))
				timeInterval = max;
			return timeInterval;
		}
		bool moreThanMin()
		{
			return currentPeriod() > min;
		}
	};
}
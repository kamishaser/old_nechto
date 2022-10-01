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
		milliseconds lastUp;

		periodLimiter(milliseconds minPeriod, milliseconds maxPeriod = 0ms)
			:min(minPeriod), lastUp(currentTime()) {}

		void reset()
		{
			lastUp = currentTime();
		}
		milliseconds currentPeriod()
		{
			milliseconds timeInterval = currentTime() - lastUp;
			return timeInterval;
		}
		bool moreThanMin()
		{
			return currentPeriod() > min;
		}
	};
}
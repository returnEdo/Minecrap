#pragma once

#include <chrono>



namespace Minecrap
{


using SteadyClock 	= std::chrono::steady_clock;
using TimePoint 	= std::chrono::time_point<SteadyClock>;

class Clock
{
	private:

	TimePoint			m_time;
	TimePoint			m_startUpTime;
	std::chrono::duration<float>	m_duration;

	public:

	Clock(void);

	float refreshTimer(void);
	float timeSinceStartUp(void);
};


}



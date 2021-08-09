#include "Clock.hpp"


namespace Minecrap
{


Clock::Clock(void):
	m_startUpTime(SteadyClock::now()),
	m_time(m_startUpTime)			{}


float Clock::refreshTimer(void)
{
	TimePoint l_now 	= SteadyClock::now();

	m_duration 	= l_now - m_time;
	m_time		= l_now;

	return m_duration.count();
}

float Clock::timeSinceStartUp(void)
{
	m_duration = SteadyClock::now() - m_startUpTime;
	
	return m_duration.count();
}


}

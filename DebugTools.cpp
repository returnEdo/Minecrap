#include "DebugTools.hpp"

#include "Constants.hpp"

namespace Minecrap
{

namespace Debug
{


float fpsCounter(float t_dt)
{
	static float s_accumulator 	= 0.0f;
	static float s_last		= 0.0f;
	static int s_counter 		= 0;

	if (s_counter++ < C_AVERAGE_WINDOW_WIDTH)
	{
		s_accumulator += t_dt;
	}
	else 
	{
		s_counter 	= 0;
		s_last 		= C_AVERAGE_WINDOW_WIDTH / s_accumulator;
		s_accumulator	= 0.0f;
	}

	return s_last;
};



}

}

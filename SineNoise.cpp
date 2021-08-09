#include "SineNoise.hpp"

#include <cmath>

namespace Noise
{



SineNoise::SineNoise(float t_period): m_period(t_period)	{}

float SineNoise::sample(float t_x, float t_y, float t_z)
{
	float t_alpha = 2.0f * M_PI/m_period;
	return 7.0f*(1+std::sin(t_alpha * t_x) * std::sin(t_alpha * t_z));
};



}

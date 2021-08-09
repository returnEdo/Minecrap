#include "Noise.hpp"

#include <cmath>
#include "MathUtils.hpp"



namespace Noise
{

// + 0.5f remaps to the [0, 1] interval
float CubicNoise::getRandomf(int32_t x, int32_t y)
{
	return static_cast<float>(((((x ^ y) * C_RAND_A) ^ (m_setup.m_seed + x)) *
				 (((C_RAND_B * x) << 16) ^ (C_RAND_B * y) - C_RAND_A))) / UINT32_MAX + 0.5f;
};

// Evaluates the normalized weight for each octave
CubicNoise::CubicNoise(NoiseSetup t_noiseSetup):	m_setup(t_noiseSetup)
{
	float l_partialSum = m_setup.m_damping * (1.0f - std::pow(m_setup.m_damping, m_setup.m_octaveCount)) / (1.0f - m_setup.m_damping);

	for (uint32_t i = 0; i < m_setup.m_octaveCount; i++)
	{
		m_weights.push_back(std::pow(m_setup.m_damping, i+1) / l_partialSum);
	}
};

// squishify
float CubicNoise::interpolate(float a, float b, float c, float d, float t_alpha)
{
	return Math::squishify(t_alpha*(t_alpha*(t_alpha*(-a+b-c+d)+2.0f*a-2.0f*b+c-d)-a+c)+b);
};

// TODO: to vec2
float CubicNoise::sampleOctave(float t_x, float t_y, uint32_t tOctave)
{
	float t_offsetx = static_cast<float>(INT32_MAX) / 2.0f;
	float t_offsety = static_cast<float>(INT32_MAX) / 2.0f;

	float x = t_x + t_offsetx;
	float y = t_y + t_offsety;

	// choose position based on the octave
	int32_t l_xi = static_cast<int32_t>(std::floor(m_setup.m_periodCountx * x * tOctave));
	int32_t l_yi = static_cast<int32_t>(std::floor(m_setup.m_periodCounty * y * tOctave));

	// find alphas
	float l_alphax = x * tOctave * m_setup.m_periodCountx - static_cast<float>(l_xi);
	float l_alphay = y * tOctave * m_setup.m_periodCounty - static_cast<float>(l_yi);

	// now we can find our samples
	std::array<float, 4> l_interpolated;

	for (int32_t l_deltay = 0; l_deltay < 4; l_deltay ++)
	{
		l_interpolated[l_deltay] = interpolate(getRandomf(l_xi - 1, l_yi + l_deltay),
						       getRandomf(l_xi    , l_yi + l_deltay),
						       getRandomf(l_xi + 1, l_yi + l_deltay),
						       getRandomf(l_xi + 2, l_yi + l_deltay),
						       l_alphax);
	}
	
	return interpolate(l_interpolated[0], l_interpolated[1],
			   l_interpolated[2], l_interpolated[3], l_alphay);
};

float CubicNoise::sample(float x, float y, float z)
{
	float l_accumulator = 0.0f;
	
	for (int i = 0; i < m_setup.m_octaveCount; i++)
	{
		l_accumulator += (m_weights[i] * sampleOctave(x, y, i+1));
	}

	return l_accumulator;
};	





}

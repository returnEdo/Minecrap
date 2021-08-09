#pragma once

#include <cstdint>
#include <vector>
#include <array>

#include "INoise.hpp"


namespace Noise
{

struct NoiseSetup
{
	uint32_t	m_octaveCount		{ 4 };
	float		m_damping		{ 0.5f };	
	int32_t 	m_seed			{ 123456 };

	float 		m_periodCountx		{ 10 };
	float		m_periodCounty		{ 10 };
};


class CubicNoise: public INoise
{
	private:

	NoiseSetup		m_setup;
	std::vector<float>	m_weights;

	static const int32_t C_RAND_A = 134775813;
	static const int32_t C_RAND_B = 1103515245;

	float interpolate(float a, float b, float c, float d, float t_alpha);

	public:

	CubicNoise(NoiseSetup t_noiseSetup);
	virtual ~CubicNoise(void) {}

	float getRandomf(int32_t x, int32_t y);

	float sampleOctave(float t_x, float t_y, uint32_t tOctave);
	virtual float sample(float x, float y, float t_z) override;

};




}

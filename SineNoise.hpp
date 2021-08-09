#pragma once 

#include "INoise.hpp"

namespace Noise
{

// Function used for debugging
class SineNoise: public INoise
{
	private:

	float m_period 	{ 10.0f };
	
	public:

	SineNoise(float t_period);

	virtual ~SineNoise(void) {}

	virtual float sample(float t_x, float t_y, float t_z) override;
};

}


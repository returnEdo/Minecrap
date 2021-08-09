#pragma once


namespace Noise
{

class INoise
{
	public:

	virtual ~INoise(void) {}

	virtual float sample(float t_x ,float t_y, float t_z) = 0;
};


}

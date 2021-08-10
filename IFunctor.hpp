#pragma once

#include "VoxelUtils.hpp"

namespace Minecrap
{

class IFunctor
{
	public:

	virtual void evaluate(Voxel& t_voxel, int x, int y, int z) = 0;
};


class SineFunctor: public IFunctor
{
	public:

	float m_period 		{ 32.0f };
	float m_amplitude 	{ 10.0f };

	// Surface below sine
	virtual void evaluate(Voxel& t_voxel, int x, int y, int z) override
	{
		float t_value = m_amplitude * (1.0f + std::sin(2.0f*M_PI*static_cast<float>(x)/m_period) * std::sin(2.0f*M_PI*static_cast<float>(z)/m_period));

		if (static_cast<float>(y) < t_value)
		{
			VoxelUtils::setActive(t_voxel);
			VoxelUtils::setType(t_voxel, BlockType::grass);
		}
	}
};


}

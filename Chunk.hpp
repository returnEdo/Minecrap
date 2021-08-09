#pragma once

#include <algorithm>
#include <array>
#include <cstdint>

#include "VoxelUtils.hpp"
#include "Constants.hpp"

namespace Minecrap
{

struct vec3i
{
	int	x { 0 };
	int	y { 0 };
	int	z { 0 };
};


using Voxels = std::array<Voxel, C_CHUNK_VOLUME>;

// Note that voxels are inactive type grass by default
struct Chunk
{
	Chunk(void)
	{
		std::fill(m_voxels.begin(), m_voxels.end(), 0);
	};

	vec3i		m_position { 0, 0, 0 }; 
	Voxels		m_voxels;

	Voxel& getVoxel(int x, int y, int z);
	Voxel& getVoxelRelative(int x, int y, int z);
};


}

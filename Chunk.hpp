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
	vec3i(int t_x, int t_y, int t_z): x(t_x), y(t_y), z(t_z)		{}
	vec3i(const vec3i& t_other): vec3i(t_other.x, t_other.y, t_other.z)	{}
	vec3i(void): vec3i(0, 0, 0)						{}

	int	x { 0 };
	int	y { 0 };
	int	z { 0 };

	friend vec3i add(vec3i a, vec3i b)
	{
		return {a.x+b.x, a.y+b.y, a.z+b.z};
	}
};


using Voxels = std::array<Voxel, C_CHUNK_VOLUME>;

// Note that voxels are inactive type grass by default
struct Chunk
{
	Chunk(void)
	{
		std::fill(m_voxels.begin(), m_voxels.end(), 0);
	};

	uint8_t		m_id;
	vec3i		m_position { 0, 0, 0 }; 
	Voxels		m_voxels;

	Voxel& getVoxel(int x, int y, int z);
	Voxel& getVoxelRelative(int x, int y, int z);
};


}

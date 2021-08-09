#include "VoxelUtils.hpp"

#include <cstdint>

#include "Macros.hpp"

namespace Minecrap
{

namespace VoxelUtils
{

// sign | normal | type | active
// leading bit = sign then components
constexpr Voxel SIGN_MASK	= 1 << 7;
constexpr Voxel X_MASK		= 1 << 6;
constexpr Voxel Y_MASK		= 1 << 5;
constexpr Voxel Z_MASK		= 1 << 4;
constexpr Voxel NORMAL_MASK	= 102;
constexpr Voxel TYPE_MASK 	= 14;
constexpr Voxel ACTIVE_MASK	= 1;


void setActive(Voxel& t_voxel)
{
	t_voxel |= 1;
}

void setInactive(Voxel& t_voxel)
{
	t_voxel &= ~(1);
}

void setType(Voxel& t_voxel, BlockType t_type)
{
	t_voxel |= (static_cast<Voxel>(t_type) << 1);
}

BlockType getType(Voxel t_voxel)
{
	return static_cast<BlockType>((t_voxel & TYPE_MASK) >> 1);
}

bool areSameType(Voxel t_voxela, Voxel t_voxelb)
{
	return (t_voxela & TYPE_MASK) == (t_voxelb & TYPE_MASK);
}

bool isActive(Voxel t_voxel)
{
	return (t_voxel & ACTIVE_MASK) == ACTIVE_MASK;
}

void setNormal(Voxel& t_voxel, int* t_normal)
{
	t_voxel |= (t_normal[0] << 6);
	t_voxel |= (t_normal[1] << 5);
	t_voxel |= (t_normal[2] << 4);
}

void setNormalSign(Voxel& t_voxel, int t_positive)
{
	t_voxel |= (t_positive << 7);	
}

void getNormal(Voxel& t_voxel, int* t_normal)
{
	int l_sign = (t_voxel & SIGN_MASK) >> 7;
	l_sign = (l_sign*2 - 1);

	t_normal[0] = l_sign * ((t_voxel & X_MASK) >> 6);
	t_normal[1] = l_sign * ((t_voxel & Y_MASK) >> 5);
	t_normal[2] = l_sign * ((t_voxel & Z_MASK) >> 4);
}


}
}

#include "VoxelUtils.hpp"

#include <cstdint>

#include "Macros.hpp"

namespace Minecrap
{

namespace VoxelUtils
{


#define GET_MASK(t_bit, t_width) (((1 << t_width) - 1) << t_bit) 

// sign | normal | type | active
constexpr uint32_t ACTIVE_BIT 	= 0;
constexpr uint32_t TYPE_BIT	= 1;
constexpr uint32_t NORMAL_BIT	= 6;
constexpr uint32_t SIGN_BIT	= 8;

constexpr uint32_t ACTIVE_WIDTH	= 1;
constexpr uint32_t TYPE_WIDTH	= NORMAL_BIT - TYPE_BIT;
constexpr uint32_t NORMAL_WIDTH	= SIGN_BIT - NORMAL_BIT;
constexpr uint32_t SIGN_WIDTH	= 1;

constexpr uint32_t ACTIVE_MASK	= GET_MASK(ACTIVE_BIT, ACTIVE_WIDTH);
constexpr uint32_t TYPE_MASK	= GET_MASK(TYPE_BIT, TYPE_WIDTH);
constexpr uint32_t NORMAL_MASK	= GET_MASK(NORMAL_BIT, NORMAL_WIDTH);
constexpr uint32_t SIGN_MASK	= GET_MASK(SIGN_BIT, SIGN_WIDTH);


void resetMask(Voxel& t_voxel, uint32_t t_mask)
{
	t_voxel &= ~t_mask;
}

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
	resetMask(t_voxel, TYPE_MASK);
	t_voxel |= (static_cast<Voxel>(t_type) << TYPE_BIT);
}

BlockType getType(Voxel t_voxel)
{
	return static_cast<BlockType>((t_voxel & TYPE_MASK) >> TYPE_BIT);
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
	resetMask(t_voxel, NORMAL_MASK);
	t_voxel |= ((t_normal[1] + 2 * t_normal[2]) << NORMAL_BIT);
}

void setNormalSign(Voxel& t_voxel, int t_positive)
{
	resetMask(t_voxel, SIGN_MASK);
	t_voxel |= (t_positive << SIGN_BIT);	
}

void getNormal(Voxel& t_voxel, int* t_normal)
{
	t_normal[0] = 0;
	t_normal[1] = 0;
	t_normal[2] = 0;

	t_normal[(t_voxel & NORMAL_MASK) >> NORMAL_BIT] = 2 * ((t_voxel & SIGN_MASK) >> SIGN_BIT) - 1;
}


}
}

#pragma once

#include <cstdint>

namespace Minecrap
{

using Voxel = uint32_t;

enum class BlockType: Voxel
{
	grass = 0,
	stone = 1,
	sand  = 2
};


namespace VoxelUtils
{

void setActive(Voxel& t_voxel);
void setInactive(Voxel& t_voxel);
bool isActive(Voxel t_voxel);

void setType(Voxel& t_voxel, BlockType t_type);
BlockType getType(Voxel t_voxel);
bool areSameType(Voxel t_voxela, Voxel t_voxelb);

void setNormal(Voxel& t_voxel, int* t_normal);
void setNormalSign(Voxel& t_voxel, int t_positive);
void getNormal(Voxel& t_voxel, int* t_normal);

}
}

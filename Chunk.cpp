#include "Chunk.hpp"


namespace Minecrap
{

// input assumed well defined (world coord)
Voxel& Chunk::getVoxel(int x, int y, int z)
{
	return m_voxels[x - m_position.x + 
			(y - m_position.y)*C_CHUNK_SIDE +
			(z - m_position.z)*C_CHUNK_SIDE*C_CHUNK_SIDE];
}

// Relative coordinates
Voxel& Chunk::getVoxelRelative(int x, int y, int z)
{
	return m_voxels[x + 
			y*C_CHUNK_SIDE +
			z*C_CHUNK_SIDE*C_CHUNK_SIDE];
}

}

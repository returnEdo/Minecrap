#include "Mesher.hpp"

#include "VoxelUtils.hpp"
#include "Constants.hpp"
#include "Macros.hpp"


namespace Minecrap
{


void Mesh::reset(void)
{
	m_vertexData.erase(m_vertexData.begin(), m_vertexData.end());
}

void Mesh::addVertex(const Graphics::VertexInput& t_vertex)
{
	m_vertexData.push_back(t_vertex);
}



using Slice = std::array<Voxel, C_CHUNK_SIDE*C_CHUNK_SIDE>;

void createMeshGreedy(Chunk& t_chunk, Mesh& t_mesh)
{
	for (int d = 0; d < 3; d++)
	{
		int u = (d+1) % 3;
		int v = (d+2) % 3;

		// slice direction
		int q[3] = {0, 0, 0};
		q[d] = 1;

		int x[3] = {0, 0, 0};

		for (x[d] = -1; x[d] < C_CHUNK_SIDE;)
		{
			Slice l_slice;

			// Populate slice voxel representation
			for (x[v] = 0; x[v] < C_CHUNK_SIDE; x[v]++)
			{
				for (x[u] = 0; x[u] < C_CHUNK_SIDE; x[u]++)
				{
					
					// check there is a discontinuity
					bool l_thisActive = (x[d] > -1?
							     VoxelUtils::isActive(t_chunk.getVoxelRelative(x[0], x[1], x[2])): false);
					bool l_nextActive = (x[d] < C_CHUNK_SIDE - 1?
							     VoxelUtils::isActive(t_chunk.getVoxelRelative(x[0]+q[0], x[1]+q[1], x[2]+q[2])): false);

					Voxel& l_voxel = l_slice[x[u] + x[v]*C_CHUNK_SIDE];
					// Surface of type this and normal in direction q
					if (l_thisActive and not l_nextActive)
					{
						VoxelUtils::setActive(l_voxel);
						VoxelUtils::setType(l_voxel, (x[d] > -1? VoxelUtils::getType(t_chunk.getVoxelRelative(x[0], x[1], x[2])): BlockType::grass));
						VoxelUtils::setNormal(l_voxel, q);
						VoxelUtils::setNormalSign(l_voxel, 1);		// positive q direction
					}
					else if (not l_thisActive and l_nextActive)
					{
						VoxelUtils::setActive(l_voxel);
						VoxelUtils::setType(l_voxel, (x[d] < C_CHUNK_SIDE-1? VoxelUtils::getType(t_chunk.getVoxelRelative(x[0]+q[0], x[1]+q[1], x[2]+q[2])): BlockType::grass));
						VoxelUtils::setNormal(l_voxel, q);
						VoxelUtils::setNormalSign(l_voxel, 0);		// negative q direction
					}
					else
					{
						l_voxel = 0;
					}
				}
			}
			
			// This is needed to place the slice in the right place
			x[d]++;

			// Let us now turn the voxel representation into a quad mesh
			int n = 0;
			for (int j = 0; j < C_CHUNK_SIDE; j++)
			{
				for (int i = 0; i < C_CHUNK_SIDE;)
				{
					if (VoxelUtils::isActive(l_slice[n]))
					{
						// We can check if the type is extending

						BlockType l_currentType = VoxelUtils::getType(l_slice[n]);

						int w;
						for (w = 1; i + w < C_CHUNK_SIDE;)
						{
							Voxel& l_next = l_slice[n + w];
							if (VoxelUtils::isActive(l_next) and VoxelUtils::getType(l_next) == l_currentType)
							{
								w++;
							}
							else
							{
								break;
							}
						}
						// Now we know the extent of w
						// Let's find the height of the quad

						bool l_stop = false;
						int h;
						for (h = 1; h + j < C_CHUNK_SIDE; h++)
						{
							for (int k = 0; k < w; k++)
							{
								Voxel& l_next = l_slice[n + k + h*C_CHUNK_SIDE];
								
								if (not VoxelUtils::isActive(l_next) or not (VoxelUtils::getType(l_next) == l_currentType))
								{
									l_stop = true;
									break;
								}
							}
							if (l_stop)
							{
								break;
							}
						}

						// Now we know the height of the quad, so we can assemble it
						
						// Update bottom left corner position
						x[u] = i;
						x[v] = j;

						// Let' find the updates
						int du[] = {0, 0, 0};
						int dv[] = {0, 0, 0};
						
						du[u] = w;
						dv[v] = h;
						
						t_mesh.addVertex({(float)x[0], (float)x[1], (float)x[2], l_slice[n]});
						t_mesh.addVertex({(float)(x[0] + du[0]), (float)(x[1] + du[1]), (float)(x[2] + du[2]), l_slice[n]});
						t_mesh.addVertex({(float)(x[0] + du[0] + dv[0]), (float)(x[1] + du[1] + dv[1]), (float)(x[2] + du[2] + dv[2]), l_slice[n]});
						t_mesh.addVertex({(float)(x[0] + dv[0]), (float)(x[1] + dv[1]), (float)(x[2] + dv[2]), l_slice[n]});

						// clear the slice
						for (int l = 0; l < h; l++)
						{
							for (int m = 0; m < w; m++)
							{
								VoxelUtils::setInactive(l_slice[n + m + l * C_CHUNK_SIDE]);
							}
						}

						i += w;
						n += w;
					}
					else
					{
						// If the current slice is not active
						i ++;		// pass to the next
						n ++;
					};
				}
			}
		}
	}
}

}

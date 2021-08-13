#include "ChunkManager.hpp"

#include "RenderingComponents.hpp"
#include "Entity.hpp"
#include "Manager.hpp"
#include "Macros.hpp"


extern ecs::Entity 	g_camera;
extern ecs::Manager	g_manager;


namespace Minecrap
{


ChunkManager::ChunkManager(void)
{
	// Filling the index queue
	for (int i = 0; i < C_CHUNK_COUNT; i++)
	{
		m_ids.push(static_cast<uint8_t>(i));
	}
}


bool ChunkManager::canDiscard(const vec3i& a, const vec3i& b) const
{
	return (std::max(std::abs(a.x-b.x), std::abs(a.z-b.z)) > C_CHUNK_SIDE * C_CHUNK_TO_LOAD / 2);
}


Voxel ChunkManager::getVoxel(int i, int j, int k)
{

	return m_pfunctor -> evaluate(i,j,k);
	vec3i l_positioni {
		static_cast<int>(std::floor(static_cast<float>(i) / C_CHUNK_SIDE))*C_CHUNK_SIDE,
		static_cast<int>(std::floor(static_cast<float>(j) / C_CHUNK_SIDE))*C_CHUNK_SIDE,
		static_cast<int>(std::floor(static_cast<float>(k) / C_CHUNK_SIDE))*C_CHUNK_SIDE
	};

	// If chunk data get voxel
	if (m_positionToId.find(l_positioni) != m_positionToId.end())
	{
		return m_chunks[m_positionToId[l_positioni]].getVoxel(i, j, k);
	}
	// Return default (air)
	else
	{
		return m_pfunctor -> evaluate(i, j, k); 
	}
}


void ChunkManager::loadChunk(const vec3i& t_position, uint8_t t_id)
{
	ASSERT(m_pfunctor != nullptr, "Load a functor for map generation!!");

	Chunk& l_chunk = m_chunks[t_id];

	l_chunk.m_position = t_position;

	m_positionToId[t_position] = t_id;

	for (int k = 0; k < C_CHUNK_SIDE; k++){
	for (int j = 0; j < C_CHUNK_SIDE; j++){
	for (int i = 0; i < C_CHUNK_SIDE; i++){

		Voxel& l_voxel = l_chunk.getVoxelRelative(i, j, k);

		VoxelUtils::reset(l_voxel);
		m_pfunctor -> evaluate(l_voxel, i + t_position.x,
						j + t_position.y,
						k + t_position.z);
	}}}
}


void ChunkManager::update(void)
{
	// Clear render list
	m_renderList.clear();

	Transform& l_camera = g_manager.getComponent<Transform>(g_camera);

	vec3i l_camerai {
		static_cast<int>(std::floor(l_camera.mPosition.x / C_CHUNK_SIDE)) * C_CHUNK_SIDE,
		0,//static_cast<int>(std::floor(l_camera.mPosition.y / C_CHUNK_SIDE)) * C_CHUNK_SIDE,
		static_cast<int>(std::floor(l_camera.mPosition.z / C_CHUNK_SIDE)) * C_CHUNK_SIDE
	};
	
	// Loop through chunks to display
	for (int k = -C_CHUNK_TO_LOAD/2; k < C_CHUNK_TO_LOAD/2; k++)
	{
		for (int i = -C_CHUNK_TO_LOAD/2; i < C_CHUNK_TO_LOAD/2; i++)
		{
			// Find the position of the chunk
			vec3i l_positioni = add(l_camerai, {i * C_CHUNK_SIDE, 0, k * C_CHUNK_SIDE});

			// Find whether it is loaded
			vec3i_to_id::iterator l_iterator = m_positionToId.find(l_positioni);
			
			uint8_t l_currentid;

			// The chunk is already loaded
			if (l_iterator != m_positionToId.end())
			{
				l_currentid = l_iterator->second;
			}
			// The chunk needs to be loaded
			else
			{
				// Still ids available
				if (not m_ids.empty())
				{
					l_currentid = m_ids.front();
					m_ids.pop();
					
				}
				// need to unload a chunk
				else 
				{
					for (int l_index = 0; l_index < C_CHUNK_COUNT; l_index++)
					{
						vec3i l_otheri = m_chunks[l_index].m_position;

						// If the chunk is outside the view
						if (canDiscard(l_camerai, l_otheri))
						{
							l_currentid = static_cast<uint8_t>(l_index);
							
							m_positionToId.erase(l_otheri);

							break;
						}
					}
				}

				// load the chunk and create the mesh
				loadChunk(l_positioni, l_currentid);
				loadMesh(l_currentid);
			//	createMeshGreedy(m_chunks[l_currentid], m_meshes[l_currentid]);
			}

			m_renderList.push_back(l_currentid);
		}
	}
}


using Slice = std::array<Voxel, C_CHUNK_SIDE*C_CHUNK_SIDE>;

void ChunkManager::loadMesh(uint8_t t_id)
{
	// Retrieve meshes and chunks
	
	Mesh& l_mesh 	= m_meshes[t_id];
	Chunk& l_chunk	= m_chunks[t_id];

	vec3i l_p = l_chunk.m_position;

	l_mesh.reset();


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


					Voxel l_thisVoxel;
					Voxel l_nextVoxel;
					if (x[d] == -1)
					{
						//l_thisVoxel = getVoxel(l_p.x + x[0], l_p.y + x[1], l_p.z + x[2]);
						l_thisVoxel = m_pfunctor->evaluate(l_p.x + x[0], l_p.y + x[1], l_p.z + x[2]);
					}
					else
					{
						l_thisVoxel = l_chunk.getVoxelRelative(x[0], x[1], x[2]);
					}
					if (x[d] == C_CHUNK_SIDE - 1)
					{
						l_nextVoxel = m_pfunctor->evaluate(l_p.x + x[0] + q[0],
								     		   l_p.y + x[1] + q[1],
								     		   l_p.z + x[2] + q[2]);
					}
					else
					{
						l_nextVoxel = l_chunk.getVoxelRelative(x[0]+q[0],x[1]+q[1],x[2]+q[2]);
					}
					

					Voxel& l_voxel = l_slice[x[u] + x[v]*C_CHUNK_SIDE];
					// Surface of type this and normal in direction q

					bool l_thisActive = VoxelUtils::isActive(l_thisVoxel);
					bool l_nextActive = VoxelUtils::isActive(l_nextVoxel);

					if ((l_thisActive and l_nextActive) or (not l_thisActive and not l_nextActive))
					{
						l_voxel = 0;
					}
					else if (l_thisActive and not l_nextActive)
					{
						VoxelUtils::setActive(l_voxel);
						VoxelUtils::setType(l_voxel, VoxelUtils::getType(l_thisVoxel));
						VoxelUtils::setNormal(l_voxel, q);
						VoxelUtils::setNormalSign(l_voxel, 1);		// positive q direction
					}
					else //if (not l_thisActive and l_nextActive)
					{
						VoxelUtils::setActive(l_voxel);
						VoxelUtils::setType(l_voxel, VoxelUtils::getType(l_nextVoxel));
						VoxelUtils::setNormal(l_voxel, q);
						VoxelUtils::setNormalSign(l_voxel, 0);		// negative q direction
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
						
						l_mesh.addVertex({(float)(x[0] + l_p.x), (float)(x[1] + l_p.y), (float)(x[2] + l_p.z), l_slice[n]});
						l_mesh.addVertex({(float)(x[0] + du[0] + l_p.x), (float)(x[1] + du[1] + l_p.y), (float)(x[2] + du[2] + l_p.z), l_slice[n]});
						l_mesh.addVertex({(float)(x[0] + du[0] + dv[0] + l_p.x), (float)(x[1] + du[1] + dv[1] + l_p.y), (float)(x[2] + du[2] + dv[2] + l_p.z), l_slice[n]});
						l_mesh.addVertex({(float)(x[0] + dv[0] + l_p.x), (float)(x[1] + dv[1] + l_p.y), (float)(x[2] + dv[2] + l_p.z), l_slice[n]});

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

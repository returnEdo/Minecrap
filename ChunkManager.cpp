#include "ChunkManager.hpp"

#include <cmath>

#include "Constants.hpp"
#include "Entity.hpp"
#include "Manager.hpp"
#include "RenderingComponents.hpp"


extern ecs::Entity 	g_camera;
extern ecs::Manager	g_manager;

namespace Minecrap
{

constexpr uint8_t NOT_PRESENT = 255;



ChunkManager::ChunkManager(void)
{
	for (int i = 0; i < C_MAX_CHUNKS - 1; i++)
	{
		m_ids.push(i);
	}
}


// Multiple things happening here:
// based on position I determine the chunks to be loaded. If I don't
// have them I generate them and I will later meshify em.
void ChunkManager::updateDisplayList(void)
{
	// clear display list
	m_displayList.clear();

	Transform& l_transform = g_manager.getComponent<Transform>(g_camera);
	
	vec3i	l_camPositioni { static_cast<int>(std::floor(l_transform.mPosition.x / C_CHUNK_SIDE)) * C_CHUNK_SIDE,
				 static_cast<int>(std::floor(l_transform.mPosition.y / C_CHUNK_SIDE)) * C_CHUNK_SIDE,
				 static_cast<int>(std::floor(l_transform.mPosition.z / C_CHUNK_SIDE)) * C_CHUNK_SIDE };
	
	for (int k = -C_CHUNK_TO_LOAD/2; k < C_CHUNK_TO_LOAD/2; k++){
	for (int i = -C_CHUNK_TO_LOAD/2; i < C_CHUNK_TO_LOAD/2; i++){

		vec3i l_chunkPosition { l_camPositioni.x + i * C_CHUNK_SIDE, 0, l_camPositioni.z + k * C_CHUNK_SIDE };

		vec3i_to_id::iterator l_iterator = m_positionToId.find(l_chunkPosition);

		// The chunk has to be created
		if (l_iterator == m_positionToId.end())
		{
			// id of the new chunk
			uint8_t l_newid;

			// Find the new id
			if (not m_ids.empty())
			{
				// Still ids available

				l_newid = m_ids.front();
				m_ids.pop();
			}
			else
			{
				// No ids available --> loop through the chunks and get rid of one of them
				for (int i = 0; i < C_MAX_CHUNKS-1; i++)
				{
					// This guy is certainly not useful
					if ((std::abs(l_camPositioni.x - m_chunks[i].m_position.x) > C_CHUNK_SIDE*C_CHUNK_TO_LOAD/2) or
					    (std::abs(l_camPositioni.z - m_chunks[i].m_position.z) > C_CHUNK_SIDE*C_CHUNK_TO_LOAD/2))
					{
						l_newid = i;
						break;
					}
				}
			}
			
			addChunk(l_chunkPosition, l_newid);
			m_displayList.push_back(l_newid);
		}
		else
		{
			// Add the chunk id to the display list
			m_displayList.push_back(l_iterator->second);
		}
	}
	}
}


void ChunkManager::addChunk(const vec3i& t_position, uint8_t t_id)
{
	Chunk& l_chunk = m_chunks[t_id];
	
	l_chunk.m_id = t_id;
	l_chunk.m_position = t_position;

	// Use the functor to populate the chunk
	for (int k = 0; k < C_CHUNK_SIDE; k++){
	for (int j = 0; j < C_CHUNK_SIDE; j++){
	for (int i = 0; i < C_CHUNK_SIDE; i++){

		Voxel& l_voxel = l_chunk.getVoxelRelative(i, j, k);
		// resetting the voxel
		VoxelUtils::reset(l_voxel);

		// use the functor to set active
		m_functor -> evaluate(l_voxel, t_position.x + i, t_position.y + j, t_position.z + k);
	}}}

	// Just created the mesh you need to meshify
	m_meshifyList.push_back(t_id);

//	std::map<vec3i, uint8_t>::iterator l_iterator;
//	vec3i l_position;

	//l_position = add(t_position, {-C_CHUNK_SIDE, 0, 0});
	//l_iterator = m_positionToId.find(l_position);
	//if (l_iterator == m_positionToId.end())	{ l_chunk.m_left = NOT_PRESENT; }
	//else 					{ l_chunk.m_left = *l_iterator; }
	//
	//l_position = add(t_position, {0, 0, -C_CHUNK_SIDE});
	//l_iterator = m_positionToId.find(l_position);
	//if (l_iterator == m_positionToId.end())	{ l_chunk.m_back = NOT_PRESENT; }
	//else 					{ l_chunk.m_back = *l_iterator; }

	//l_position = add(t_position, {0, -C_CHUNK_SIDE, 0});
	//l_iterator = m_positionToId.find(l_position);
	//if (l_iterator == m_positionToId.end())	{ l_chunk.m_bottom = NOT_PRESENT; }
	//else 					{ l_chunk.m_bottom = *l_iterator; }
}


void ChunkManager::updateMesh(void)
{
	for (uint8_t l_id: m_meshifyList)
	{
		createMeshGreedy(m_chunks[l_id], m_meshes[l_id]);
	}
	m_meshifyList.clear();
}

}

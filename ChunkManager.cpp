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
				createMeshGreedy(m_chunks[l_currentid], m_meshes[l_currentid]);
			}

			m_renderList.push_back(l_currentid);
		}
	}
}


}

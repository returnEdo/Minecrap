#pragma once

#include <queue>
#include <cstdint>
#include <array>
#include <map>

#include "Chunk.hpp"
#include "Mesher.hpp"
#include "IFunctor.hpp"


namespace Minecrap
{

struct Compare_vec3i
{
	bool operator ()(const vec3i a, const vec3i& b)	
	{
		return (a.x < b.x or (a.x == b.x and 
			(a.y < b.y or (a.y == b.y and a.z < b.z))));
	}
};


using  vec3i_to_id = std::map<vec3i, uint8_t, Compare_vec3i>;

class ChunkManager
{
	private:	

	std::queue<uint8_t>			m_ids;
	// -1 because value 255 is reserved 
	std::array<Chunk, C_MAX_CHUNKS-1>	m_chunks;
	std::array<Mesh, C_MAX_CHUNKS-1>	m_meshes;
	
	vec3i_to_id				m_positionToId;

	std::vector<uint8_t>			m_displayList;
	std::vector<uint8_t>			m_meshifyList;

	public:

	// For terrain generation
	IFunctor* 				m_functor { nullptr };

	ChunkManager(void);

	void updateDisplayList(void);
	void updateMesh(void);
	void addChunk(const vec3i& t_position, uint8_t t_id);
};

}

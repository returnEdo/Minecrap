#pragma once


#include <map>
#include <queue>
#include <array>

#include "Chunk.hpp"
#include "Mesher.hpp"
#include "IFunctor.hpp"
#include "Constants.hpp"


namespace Minecrap
{

constexpr int C_CHUNK_COUNT = 1 << 8;


struct comparator
{
	bool operator ()(const vec3i a, const vec3i& b)	
	{
		return (a.x < b.x or (a.x == b.x and 
		       (a.y < b.y or (a.y == b.y and a.z < b.z))));
	}
};

using vec3i_to_id = std::map<vec3i, uint8_t, comparator>;

class ChunkManager
{
	private:

	bool canDiscard(const vec3i& a, const vec3i& b) const;
	Chunk* getChunkp(vec3i t_position);
	Voxel getVoxel(int i, int j, int k);

	public:

	std::queue<uint8_t>			m_ids;
	std::array<Chunk, C_CHUNK_COUNT>	m_chunks;
	std::array<Mesh, C_CHUNK_COUNT>		m_meshes;

	std::vector<uint8_t>			m_renderList;

	vec3i_to_id				m_positionToId;

	IFunctor*				m_pfunctor { nullptr };

	ChunkManager(void);

	void loadChunk(const vec3i& t_position, uint8_t t_id);
	void loadMesh(uint8_t t_id);
	void update(void);
};

}

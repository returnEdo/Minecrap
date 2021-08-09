#pragma once

#include "Renderer.hpp"
#include "Chunk.hpp"

namespace Minecrap
{

struct Mesh
{
	std::vector<Graphics::VertexInput>	m_vertexData;

	void reset(void);
	void addVertex(const Graphics::VertexInput& t_vertex);
};

void createMeshGreedy(Chunk& t_chunk, Mesh& t_mesh);

}

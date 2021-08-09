#pragma once

#include <vector>
#include <string>
#include <cstdint>

#include "Shader.hpp"
#include "vec2.hpp"
#include "vec3.hpp"


namespace Minecrap
{

namespace Graphics
{

struct VertexInput
{
	float		m_position[3];
	uint32_t 	m_packed;
};

// Shaders and texture addresses
const std::string C_VERTEX_ADDRESS 	= "./resources/shaders/basic.vs";	 
const std::string C_FRAGMENT_ADDRESS	= "./resources/shaders/basic.fs";

constexpr int C_MAX_VERTICES		= 10000;

class Renderer
{
	private:

	Graphics::Shader	mShader;
	
	unsigned int		mVertexArrayId;
	unsigned int		mVertexBufferId;

	public:

	void init(void);

	void updateUniforms(void);
	void render(std::vector<Graphics::VertexInput>& t_vertexInput);

	void destroy(void);
};

}

}

#pragma once

#include <GL/glew.h>
#include "Shader.hpp"


namespace Minecrap
{


namespace Graphics
{


const std::string C_QUAD_VERTEX 	= "./resources/shaders/quad.vs";
const std::string C_QUAD_FRAGMENT	= "./resources/shaders/quad.fs";


class QuadRenderer
{
	private:

	unsigned int	m_vertexBuffer;
	unsigned int 	m_indexBuffer;
	unsigned int 	m_vertexArray;

	Graphics::Shader	m_shader;

	public:
	
	void init(void);

	void render(unsigned int t_texture0);//, unsigned int t_texture1);

	void destroy(void);
};


}

}

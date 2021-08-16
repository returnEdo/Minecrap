#include "QuadRenderer.hpp"


namespace Minecrap
{

namespace Graphics
{


void QuadRenderer::init(void)
{
	// to be rewritten: note texture = vertex coordinates
	float l_vertex[] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f, 
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	
	unsigned int l_index[] = { 0, 1, 2, 0, 2, 3 };

	m_shader.init(C_QUAD_VERTEX, C_QUAD_FRAGMENT);
	m_shader.findUniformLocations({"s_texture0"});
	//m_shader.findUniformLocations({"s_texture0", "s_texture1"});

	// Set the texture units
	m_shader.setUniform("s_texture0", 0);
//	m_shader.setUniform("s_texture0", 1);
	
	glGenVertexArrays(1, &m_vertexArray);
	glGenBuffers(1, &m_vertexBuffer);
	glGenBuffers(1, &m_indexBuffer);

	glBindVertexArray(m_vertexArray);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(l_vertex), l_vertex, GL_STATIC_DRAW);	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(l_index), l_index, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void QuadRenderer::render(unsigned int t_texture0)//, unsigned int t_texture1)
{
	m_shader.bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t_texture0);
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, t_texture1);
		
	glBindVertexArray(m_vertexArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void QuadRenderer::destroy(void)
{
	glDeleteVertexArrays(1, &m_vertexArray);
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_indexBuffer);
	
	m_shader.destroy();
}

}

}

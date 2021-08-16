#include "Renderer.hpp"

#include <GL/glew.h>
#include <cstddef>
#include <cstring>
#include <cmath>

#include "Entity.hpp"
#include "Manager.hpp"
#include "Macros.hpp"
#include "RenderingComponents.hpp"


extern ecs::Manager 	g_manager;
extern ecs::Entity	g_camera;


namespace Minecrap
{

namespace Graphics
{

void Renderer::init(void)
{
	mShader.init(C_VERTEX_ADDRESS, C_FRAGMENT_ADDRESS);

	mShader.findUniformLocations({"u_camNear", "u_camFar", "u_camTan",
				      "u_camAlpha", "u_camPosition", "u_camRotationt"});

	glGenVertexArrays(1, &mVertexArrayId);
	glGenBuffers(1, &mVertexBufferId);

	glBindVertexArray(mVertexArrayId);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexInput) * C_MAX_VERTICES, nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, COUNT(VertexInput::m_position, float), GL_FLOAT, GL_FALSE, sizeof(VertexInput), (void*)offsetof(VertexInput, m_position));
	glEnableVertexAttribArray(0);
	glVertexAttribIPointer(1, COUNT(VertexInput::m_packed, uint32_t), GL_UNSIGNED_INT, sizeof(VertexInput), (void*)offsetof(VertexInput, m_packed));
	glEnableVertexAttribArray(1);
}


void Renderer::updateUniforms(void)
{
	Transform l_transform	= g_manager.getComponent<Transform>(g_camera);
	View l_view 		= g_manager.getComponent<View>(g_camera);

	mShader.bind();

	mShader.setUniform("u_camNear", l_view.m_nearPlane);
	mShader.setUniform("u_camFar", l_view.m_farPlane);
	mShader.setUniform("u_camTan", std::tan(l_view.m_fov / 2.0f));
	mShader.setUniform("u_camAlpha", l_view.m_alpha);
	mShader.setUniform("u_camPosition", l_transform.mPosition);
	Math::mat3 l_rotationt = transpose(l_transform.mRotor.getMatrixForm());
	mShader.setUniform("u_camRotationt", l_rotationt);
}


void Renderer::render(std::vector<Graphics::VertexInput>& t_vertexInput)
{
	WARNING(t_vertexInput.size() > C_MAX_VERTICES, "Too many vertices; chopped off the excess!!");

	glBindVertexArray(mVertexArrayId);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);

	void* l_vertexTarget = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(l_vertexTarget, t_vertexInput.data(), std::min((int)t_vertexInput.size(), C_MAX_VERTICES) * sizeof(VertexInput));

	glUnmapBuffer(GL_ARRAY_BUFFER);

	glDrawArrays(GL_QUADS, 0, t_vertexInput.size());
}


void Renderer::destroy(void)
{
	glDeleteVertexArrays(1, &mVertexArrayId);
	glDeleteBuffers(1, &mVertexBufferId);

	mShader.destroy();
}


}

}

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "WindowManager.hpp"
#include "InputManager.hpp"
#include "Macros.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Chunk.hpp"
#include "ChunkManager.hpp"
#include "VoxelUtils.hpp"
#include "Mesher.hpp"
#include "IFunctor.hpp"

#include "RenderingComponents.hpp"
#include "Frustum.hpp"
#include "Manager.hpp"

#include "Controller.hpp"
#include "Clock.hpp"
#include "DebugTools.hpp"

#include "vec2.hpp"
#include "vec3.hpp"
#include "Rotor.hpp"
#include "MathUtils.hpp"

#include "FrameBuffer.hpp"
#include "QuadRenderer.hpp"

// Globals declarations

// Manager of the ecs system
ecs::Manager 		g_manager;
ecs::Entity		g_camera;


// Window and input classes
Minecrap::WindowManager g_windowManager;
Minecrap::InputManager	g_inputManager;

using namespace Minecrap;

int main()
{
	g_manager.init();

	// Camera entity
	g_camera = g_manager.createEntity();
	Transform& l_cameraTransform = g_manager.addComponent<Transform>(g_camera,
									 {			
										Math::vec3(0.0f, 0.0f, 40.0f)
									 });		
	Controller::lookAt(Math::vec3(0.0f));
	View& lView		= g_manager.addComponent<View>(g_camera, {});
	Frustum& l_frustum	= g_manager.addComponent<Frustum>(g_camera, {});

	g_windowManager.init("Minecrap");
	g_inputManager.init(g_windowManager.getWindow());

	Graphics::QuadRenderer l_quadRenderer;
	l_quadRenderer.init();

	Graphics::Renderer l_renderer;
	l_renderer.init();

	// Create a chunk
	float l_theta 	= 0.0f;
	float l_dt	= 0.1f;
	float l_radius  = 20.0f;

	Clock l_clock;
	int l_counter = 0;


	SineFunctor l_functor;
	l_functor.m_period = 50 + M_PI;
	l_functor.m_amplitude = 5;
	ChunkManager l_chunkManager;
	l_chunkManager.m_pfunctor = &l_functor;

//	while (!g_windowManager.shouldDie())
//	{
//		Controller::updateCamera();
//		l_chunkManager.update();
//
//		if (g_inputManager.isPressed(GLFW_KEY_ESCAPE))
//		{	
//			g_windowManager.closeWindow();		
//		};
//
//		g_windowManager.clear();
//
//		l_renderer.updateUniforms();
//		for (uint8_t l_index: l_chunkManager.m_renderList)
//		{
//			l_renderer.render(l_chunkManager.m_meshes[l_index].m_vertexData);
//		}
//
//		g_windowManager.update();
//	}

	unsigned int m_frameBuffer;
	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	// create a color attachment texture
	unsigned int m_color;
	glGenTextures(1, &m_color);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_windowManager.mWidth, g_windowManager.mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, C_WINDOW_WIDTH, C_WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int m_depth;
	glGenTextures(1, &m_depth);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, C_WINDOW_WIDTH, C_WINDOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		PRINT("NOT ocmpolete");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	while (!g_windowManager.shouldDie())
	{
		Controller::updateCamera();
		l_chunkManager.update();

		if (g_inputManager.isPressed(GLFW_KEY_ESCAPE))
		{	
			g_windowManager.closeWindow();		
		};

//		g_windowManager.clear();

		// First pass: offscreen rendering
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		l_renderer.updateUniforms();
		for (uint8_t l_index: l_chunkManager.m_renderList)
		{
			l_renderer.render(l_chunkManager.m_meshes[l_index].m_vertexData);
		}

		// Second pass: render to quad
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		l_quadRenderer.render(m_color);//, l_frameBuffer.getDepthTextureId());

		g_windowManager.update();
	}

	l_renderer.destroy();
	l_quadRenderer.destroy();

	g_windowManager.destroy();
	return 0;
}



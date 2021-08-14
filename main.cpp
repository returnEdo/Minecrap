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

	Minecrap::Graphics::Renderer l_renderer;
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

	while (!g_windowManager.shouldDie())
	{
		Controller::updateCamera();
		l_chunkManager.update();

		if (g_inputManager.isPressed(GLFW_KEY_ESCAPE))
		{	
			g_windowManager.closeWindow();		
		};

		g_windowManager.clear();

		l_renderer.updateUniforms();
		for (uint8_t l_index: l_chunkManager.m_renderList)
		{
			l_renderer.render(l_chunkManager.m_meshes[l_index].m_vertexData);
		}

		g_windowManager.update();
	}

	l_renderer.destroy();
	g_windowManager.destroy();
	return 0;
}



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
#include "VoxelUtils.hpp"
#include "Mesher.hpp"
#include "SineNoise.hpp"

#include "RenderingComponents.hpp"
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
	View& lView	= g_manager.addComponent<View>(g_camera, {});

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

	Chunk l_chunk;
	Mesh l_mesh;


	for (int k = 1; k < C_CHUNK_SIDE; k++){
	for (int j = 1; j < C_CHUNK_SIDE; j++){
	for (int i = 1; i < C_CHUNK_SIDE; i++){

		if (Math::randf(.0f, 1.0f) > 0.01f)
		{
			VoxelUtils::setActive(l_chunk.m_voxels[i + j*C_CHUNK_SIDE + k*C_CHUNK_SIDE*C_CHUNK_SIDE]);
			if (j > 4)
				{VoxelUtils::setType(l_chunk.m_voxels[i + j*C_CHUNK_SIDE + k*C_CHUNK_SIDE*C_CHUNK_SIDE], BlockType::sand);}
			if (j > 8)
				{VoxelUtils::setType(l_chunk.m_voxels[i + j*C_CHUNK_SIDE + k*C_CHUNK_SIDE*C_CHUNK_SIDE], BlockType::stone);}
		}
	}}}


	createMeshGreedy(l_chunk, l_mesh);

	while (!g_windowManager.shouldDie())
	{
		Controller::updateCamera();

		if (g_inputManager.isPressed(GLFW_KEY_ESCAPE))
		{	
			g_windowManager.closeWindow();		
		};

		g_windowManager.clear();

		l_renderer.updateUniforms();
		l_renderer.render(l_mesh.m_vertexData);

		g_windowManager.update();
	}

	l_renderer.destroy();
	g_windowManager.destroy();
	return 0;
}



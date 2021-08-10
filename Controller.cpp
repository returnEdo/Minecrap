#include "Controller.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Entity.hpp"
#include "Manager.hpp"
#include "InputManager.hpp"
#include "WindowManager.hpp"
#include "RenderingComponents.hpp"
#include "Constants.hpp"
#include "MathConstants.hpp"
#include "MathUtils.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

extern ecs::Manager 		g_manager;
extern ecs::Entity		g_camera;
extern Minecrap::InputManager	g_inputManager;
extern Minecrap::WindowManager	g_windowManager;

namespace Minecrap
{

namespace Controller
{

void updateCamera(void)
{
	static float s_thetay = 0.0f;
	static float s_thetax = 0.0f;
	static Math::vec2 s_previousMousePosition { 0.0f, 0.0f };

	Transform& l_transform  = g_manager.getComponent<Transform>(g_camera); 
	View& l_view		= g_manager.getComponent<View>(g_camera);

	Math::vec3 l_deltaPosition(0.0f);

//	if (g_inputManager.isDown(GLFW_KEY_RIGHT))	
//	if (g_inputManager.isDown(GLFW_KEY_LEFT))	
//	if (g_inputManager.isDown(GLFW_KEY_UP))		
//	if (g_inputManager.isDown(GLFW_KEY_DOWN))	
//	if (g_inputManager.isDown(GLFW_KEY_PAGE_UP))	
//	if (g_inputManager.isDown(GLFW_KEY_PAGE_DOWN))	

	if (g_inputManager.isDown(GLFW_KEY_A))		{ l_deltaPosition.x -= C_DELTA_POSITION; }
	if (g_inputManager.isDown(GLFW_KEY_D))		{ l_deltaPosition.x += C_DELTA_POSITION; }
	if (g_inputManager.isDown(GLFW_KEY_W))		{ l_deltaPosition.z -= C_DELTA_POSITION; }
	if (g_inputManager.isDown(GLFW_KEY_S))		{ l_deltaPosition.z += C_DELTA_POSITION; }
	if (g_inputManager.isDown(GLFW_KEY_Q))		{ l_deltaPosition.y += C_DELTA_POSITION; }
	if (g_inputManager.isDown(GLFW_KEY_E))		{ l_deltaPosition.y -= C_DELTA_POSITION; }

	if (g_inputManager.isDown(GLFW_KEY_P))		{ glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
	if (g_inputManager.isDown(GLFW_KEY_L))		{ glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

	// Mouse control
	float l_mousex = g_inputManager.mMousePosition.x - g_windowManager.mWidth / 2.0f;
	float l_mousey = -g_inputManager.mMousePosition.y + g_windowManager.mHeight / 2.0f;
	
	float k = 1.0f / 10000.0f;
	float delta = 100.0f;
	
	s_thetay -= Math::sign<float>(l_mousex) * std::max(0.0f, k * (std::abs(l_mousex) - delta));
	s_thetax += Math::sign<float>(l_mousey) * std::max(0.0f, k * (std::abs(l_mousey) - delta));

	s_thetax = Math::clamp<float>(s_thetax, -M_PI/2.0f + C_OFFSET_ANGLE, M_PI/2.0f - C_OFFSET_ANGLE);

	Math::Rotor l_rotory(s_thetay, Math::Y_AXIS);
	Math::Rotor l_rotorx(s_thetax, Math::X_AXIS);

	l_transform.mPosition += l_rotory.getMatrixForm() * l_deltaPosition;
	l_transform.mRotor = Math::composeRotors(l_rotorx, l_rotory);
};

void lookAt(const Math::vec3& t_focus)
{
	Transform& l_transform = g_manager.getComponent<Transform>(g_camera);

	Math::vec3 lUp {0.0f, 1.0f, 0.0f};

	Math::vec3 lxCam;
	Math::vec3 lyCam;
	Math::vec3 lzCam;

	lzCam = l_transform.mPosition - t_focus;
	lzCam.normalize();

	lxCam = lUp ^ lzCam;
	lxCam.normalize();

	lyCam = lzCam ^ lxCam;

	Math::mat3 lRotationMatrix = transpose(Math::mat3(lxCam, lyCam, lzCam));

	l_transform.mRotor = Math::Rotor(lRotationMatrix);
};


}

}

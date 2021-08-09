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

	if (g_inputManager.isDown(GLFW_KEY_RIGHT))	{ l_deltaPosition.x += C_DELTA_POSITION; }
	if (g_inputManager.isDown(GLFW_KEY_LEFT))	{ l_deltaPosition.x -= C_DELTA_POSITION; }
	if (g_inputManager.isDown(GLFW_KEY_UP))		{ l_deltaPosition.z -= C_DELTA_POSITION; }
	if (g_inputManager.isDown(GLFW_KEY_DOWN))	{ l_deltaPosition.z += C_DELTA_POSITION; }
	if (g_inputManager.isDown(GLFW_KEY_PAGE_UP))	{ l_deltaPosition.y += C_DELTA_POSITION; }
	if (g_inputManager.isDown(GLFW_KEY_PAGE_DOWN))	{ l_deltaPosition.y -= C_DELTA_POSITION; }

	if (g_inputManager.isDown(GLFW_KEY_A))		{ s_thetay += C_DELTA_ANGLE; }
	if (g_inputManager.isDown(GLFW_KEY_D))		{ s_thetay -= C_DELTA_ANGLE; }
	if (g_inputManager.isDown(GLFW_KEY_W))		{ s_thetax += C_DELTA_ANGLE; }
	if (g_inputManager.isDown(GLFW_KEY_S))		{ s_thetax -= C_DELTA_ANGLE; }

	if (g_inputManager.isDown(GLFW_KEY_P))		{ glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
	if (g_inputManager.isDown(GLFW_KEY_L))		{ glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

	// Mouse rotation
	Math::vec2 l_deltaMouse = g_inputManager.mMousePosition - s_previousMousePosition;

	// In cam coordinates
	Math::vec2 l_this { g_inputManager.mMousePosition.x - g_windowManager.mWidth / 2.0f, -g_inputManager.mMousePosition.y + g_windowManager.mHeight / 2.0f };
	Math::vec2 l_prev { s_previousMousePosition.x - g_windowManager.mWidth / 2.0f, -s_previousMousePosition.y + g_windowManager.mHeight / 2.0f };
	
	l_this *= 2.0f * l_view.m_nearPlane * std::atan(l_view.m_fov / 2.0f) / g_windowManager.mWidth;
	l_prev *= 2.0f * l_view.m_nearPlane * std::atan(l_view.m_fov / 2.0f) / g_windowManager.mWidth;

	Math::vec2 l_deltaC = l_this - l_prev;

	s_thetax -= std::atan(l_deltaC.y / l_view.m_nearPlane)/3.0f;
	s_thetay += std::atan(l_deltaC.x / l_view.m_nearPlane)/3.0f;

	s_previousMousePosition = g_inputManager.mMousePosition;


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

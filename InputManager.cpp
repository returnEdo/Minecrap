#include "InputManager.hpp"

#include "Macros.hpp"



namespace Minecrap
{

std::unordered_map<int, int> 	InputManager::mKeyState;
std::unordered_map<int, int> 	InputManager::mMouseState;
Math::vec2			InputManager::mMousePosition;

void InputManager::init(GLFWwindow* tWindow)
{
	glfwSetKeyCallback(		tWindow, InputManager::keyboardCallback);
	glfwSetMouseButtonCallback(	tWindow, InputManager::mouseButtonsCallback);
	glfwSetCursorPosCallback(	tWindow, InputManager::mousePositionCallback);
};

void InputManager::keyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mode)
{
	InputManager::mKeyState[key] = action;
};

void InputManager::mouseButtonsCallback(GLFWwindow* win, int button, int action, int mode)
{
	InputManager::mMouseState[button] = action;
};

void InputManager::mousePositionCallback(GLFWwindow* win, double xpos, double ypos)
{
	InputManager::mMousePosition.x = static_cast<float>(xpos);
	InputManager::mMousePosition.y = static_cast<float>(ypos);
};

bool InputManager::isPressed(int t_key) const
{
	return InputManager::mKeyState[t_key] == GLFW_PRESS;
};

bool InputManager::isHeld(int t_key) const
{
	return InputManager::mKeyState[t_key] == GLFW_REPEAT;
};

bool InputManager::isDown(int t_key) const
{
	int l_state = InputManager::mKeyState[t_key];

	return (l_state == GLFW_PRESS or l_state == GLFW_REPEAT); 
};

};

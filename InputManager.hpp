#pragma once

#include <unordered_map>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vec2.hpp"

namespace Minecrap
{


class InputManager
{
	public:

	static std::unordered_map<int, int> 	mKeyState;
	static std::unordered_map<int, int> 	mMouseState;
	static Math::vec2			mMousePosition;

	void init(GLFWwindow* tWindow);

	static void keyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mode);
	static void mouseButtonsCallback(GLFWwindow* win, int button, int action, int mode);
	static void mousePositionCallback(GLFWwindow* win, double xpos, double ypos);

	bool isPressed(int t_key) const;
	bool isHeld(int t_key) const;
	bool isDown(int t_key) const;
};


};

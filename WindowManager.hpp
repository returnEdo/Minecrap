#pragma once

#include <unordered_map>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vec3.hpp"
#include "vec2.hpp"
#include "Constants.hpp"

namespace Minecrap
{


class WindowManager
{
	private:

	GLFWwindow*	mWindow { nullptr };

	public:
	
	static int mWidth;
	static int mHeight;

	void init(const std::string& tTitle);
	void destroy(void);

	void clear(void);
	void update(void);

	bool shouldDie(void);
	void closeWindow(void);

	static void windowSizeCallback(GLFWwindow* win, int width, int height);

	inline GLFWwindow* getWindow(void) { return mWindow; }
};


}

#include "WindowManager.hpp"

#include "Macros.hpp"
#include "Manager.hpp"
#include "Entity.hpp"
#include "RenderingComponents.hpp"

extern ecs::Manager 	g_manager;
extern ecs::Entity	g_camera;


namespace Minecrap
{

int WindowManager::mWidth 	= C_WINDOW_WIDTH;
int WindowManager::mHeight	= C_WINDOW_HEIGHT;

void WindowManager::init(const std::string& tTitle)
{
	glfwInit();

	mWindow = glfwCreateWindow(C_WINDOW_WIDTH, C_WINDOW_HEIGHT, tTitle.c_str(), nullptr, nullptr);
	
	ASSERT(mWindow != nullptr, "Could not create window!!");

	glfwMakeContextCurrent(mWindow);

	ASSERT(!glewInit(), "Could not load OpenGL functions!!");

	glfwSetWindowSizeCallback(mWindow, WindowManager::windowSizeCallback);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
};

void WindowManager::destroy(void)
{
	glfwTerminate();
};

void WindowManager::clear(void)
{
	glClearColor(C_BK_COLOR.x, C_BK_COLOR.y, C_BK_COLOR.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};

void WindowManager::update(void)
{
	glfwSwapBuffers(mWindow);
	glfwPollEvents();		// updates callback
};

bool WindowManager::shouldDie(void)
{
	return glfwWindowShouldClose(mWindow);
};

void WindowManager::closeWindow(void)
{	
	glfwSetWindowShouldClose(mWindow, true);
};


// Remaps the screen after a resizing
void WindowManager::windowSizeCallback(GLFWwindow* win, int width, int height)
{	
	glViewport(0, 0, width, height);

	View& l_view = g_manager.getComponent<View>(g_camera);

	l_view.m_alpha= static_cast<float>(width) / static_cast<float>(height);

	WindowManager::mWidth 	= width;
	WindowManager::mHeight = height;
};

}

#include "Window.h"

using namespace std;

Window* Window::Instance = NULL;

Window* Window::GetInstance()
{
	if (!Instance)
	{
		Instance = new Window();
	}
	return Instance;
}

Window::Window()
{
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
}

Window::~Window()
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}
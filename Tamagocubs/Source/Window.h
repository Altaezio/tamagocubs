#pragma once
#include <GLFW/glfw3.h>

class Window
{
public:
	static Window* GetInstance();

	GLFWwindow* window;

private:
	Window();
	~Window();

	static Window* Instance;
};


#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Window.h"
#include "CoreGame/Tamagocub.h"
#include "Renderer/CubRenderer.h"
#include <chrono>

int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	Window* myWindow = Window::GetInstance();
	GLFWwindow* window = myWindow->window;

	/* Create a windowed mode window and its OpenGL context */
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Tamagocub* tamagocub = new Tamagocub();
	CubRenderer* cubRenderer = new CubRenderer(myWindow, tamagocub);

	myWindow->InitialiseVerticies();
	myWindow->SetNewMiddleTexture("Idle.png");

	std::chrono::time_point<std::chrono::system_clock> then = std::chrono::system_clock::now();
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
		float deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - then).count() / 1000.0f;
		myWindow->Draw();
		tamagocub->Update(deltaTime);
		then = now;
	}

	glfwTerminate();
	return 0;
}

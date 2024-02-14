#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Window.h"
#include "CoreGame/Tamagocub.h"
#include "Renderer/CubRenderer.h"
#include <chrono>
#include <iostream>
#include "KeyInput.h"
#include "DataSaver.h"

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

	Tamagocub* tamagocub = DataSaver::LoadOrCreateTamagocub();
	if (!tamagocub)
	{
		std::cout << "ERROR : loading tamagocub" << std::endl;
		return -1;
	}

	CubRenderer* cubRenderer = new CubRenderer(myWindow, tamagocub);

	KeyInput* keyInput = KeyInput::InitialiseInputs(myWindow, tamagocub);

	myWindow->InitialiseVerticies();
	//myWindow->SetNewMiddleTexture("Idle.png");

	double then = glfwGetTime();
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		double now = glfwGetTime();
		float deltaTime = now - then;
		myWindow->Draw();
		tamagocub->Update(deltaTime);
		cubRenderer->Update(deltaTime);
		then = now;
	}

	DataSaver::SaveProgress(tamagocub);

	glfwTerminate();
	return 0;
}

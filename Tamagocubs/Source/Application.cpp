#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Renderer/Window.h"
#include "CoreGame/Tamagocub.h"
#include "Renderer/CubRenderer.h"
#include <chrono>
#include <iostream>
#include "KeyInput.h"
#include "DataSaver.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Tamagocub* tamagocub = DataSaver::LoadOrCreateTamagocub();
	if (!tamagocub)
	{
		std::cout << "ERROR : loading tamagocub" << std::endl;
		return -1;
	}

	CubRenderer* cubRenderer = new CubRenderer(myWindow, tamagocub);

	KeyInput* keyInput = KeyInput::InitialiseInputs(myWindow, tamagocub);

	myWindow->InitialiseTextShader();
	myWindow->InitialiseVerticies();
	myWindow->LoadTextures();

	double then = glfwGetTime();
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		double now = glfwGetTime();
		float deltaTime = (float)(now - then);
		tamagocub->Update(deltaTime);

		myWindow->StartRendering();
		cubRenderer->Draw(deltaTime);
		myWindow->StopRendering();
		then = now;
	}

	DataSaver::SaveProgress(tamagocub);

	glfwTerminate();
	return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

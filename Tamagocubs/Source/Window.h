#pragma once
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>

class Shader;

class Window
{
public:
	static Window* GetInstance();

	GLFWwindow* window;

	const unsigned int WindowWidth = 640, WindowHeight = 640;

	void InitialiseVerticies();

	void Draw();

	void SetNewMiddleTexture(std::string);

private:
	Window();
	~Window();

	unsigned int VBO, VAO, EBO;
	Shader* shader;
	unsigned int middleTexture;

	static Window* Instance;
};


#pragma once
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>
#include <map>
#include <glm.hpp>

class Shader;
class TextRenderer;
class Texture;

class Window
{
public:
	static Window* GetInstance();

	GLFWwindow* window;

	const unsigned int WindowWidth = 640, WindowHeight = 640;

	void InitialiseVerticies();
	void InitialiseTextShader();

	Texture& LoadTexture(const char* file, std::string name);
	void LoadTextures();

	void StartRendering();
	void StopRendering();

	void DrawImage(std::string textureName, glm::vec2 position, glm::vec2 scale = glm::vec2(1.0), float rotation = 0.0f, glm::vec3 color = glm::vec3(1.0));

	TextRenderer* textRenderer;

private:
	Window();
	~Window();

	unsigned int VAO;
	Shader* shader;
	std::map<std::string, Texture> textures;

	static Window* Instance;
};


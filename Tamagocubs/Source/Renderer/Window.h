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
class AnimatedTexture;

class Window
{
public:
	static Window* GetInstance();

	GLFWwindow* window;

	const unsigned int WindowWidth = 640, WindowHeight = 640;

	void InitialiseVerticies();
	void InitialiseTextShader();

	Texture& LoadTexture(const char* file, std::string name);
	AnimatedTexture& LoadAnimatedTexture(const char* file, std::string name, unsigned spriteWidth, unsigned spriteHeight, float framePerSec);
	void LoadTextures();

	void StartRendering();
	void StopRendering();

	void DrawImage(std::string textureName, glm::vec2 position, glm::vec2 size, float rotation = 0.0f, glm::vec3 color = glm::vec3(1.0), bool keepRatio = false);
	void DrawAnimatedImage(float deltaTime, std::string textureName, glm::vec2 position, glm::vec2 size, float rotation = 0.0f, glm::vec3 color = glm::vec3(1.0), bool keepRatio = false);
	void ResetAnimation(std::string textureName);

	const AnimatedTexture& GetAnimTexture(std::string name);

	TextRenderer* textRenderer;

private:
	Window();
	~Window();

	unsigned int staticVAO, animatedVAO;
	Shader* shader;
	std::map<std::string, Texture> textures;
	std::map<std::string, AnimatedTexture> animatedTextures;

	static Window* Instance;
};


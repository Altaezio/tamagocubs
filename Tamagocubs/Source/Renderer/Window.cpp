#include "Window.h"
#include "Shader.h"
#include "TextRenderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <filesystem>
#include <glm.hpp>
#include "Texture.h"

using namespace std;

Window* Window::Instance = nullptr;

Window* Window::GetInstance()
{
	if (!Instance)
	{
		Instance = new Window();
	}
	return Instance;
}

void Window::InitialiseVerticies()
{
	shader = new Shader("Source\\Renderer\\vertices.vs", "Source\\Renderer\\fragments.fs");
	shader->setMatrix4("projection", glm::ortho(0.0f, static_cast<float>(WindowWidth), static_cast<float>(WindowHeight), 0.0f, -1.0f, 1.0f), true);
	shader->setInt("sprite", 0, true);
	// configure VAO/VBO
	unsigned int VBO;
	float vertices[] = {
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Window::InitialiseTextShader()
{
	textRenderer = new TextRenderer(WindowWidth, WindowHeight);
	textRenderer->Load("Source\\Renderer\\font.TTF", 24);
}

Texture& Window::LoadTexture(const char* file, std::string name)
{
	Texture texture;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	if (data)
	{
		// now generate texture
		texture.Generate(width, height, data);
	}
	else
	{
		cout << "Error loading texture : \"" << file << "\"" << endl;
	}
	// and finally free image data
	stbi_image_free(data);
	textures[name] = texture;
	return texture;
}

void Window::LoadTextures()
{
	LoadTexture("Resources\\Static\\Idle.png", "idle");
	LoadTexture("Resources\\Static\\Hungry.png", "hungry");
	LoadTexture("Resources\\Static\\Sick.png", "sick");
	LoadTexture("Resources\\Static\\Wet.png", "wet");
	LoadTexture("Resources\\Static\\WontDo.png", "wontDo");
	LoadTexture("Resources\\Static\\Dispute.png", "dispute");
	LoadTexture("Resources\\Static\\Fed.png", "fed");
	LoadTexture("Resources\\Static\\Heal.png", "heal");
	LoadTexture("Resources\\Static\\Clean.png", "clean");
	LoadTexture("Resources\\Static\\Sport.png", "sport");
}

void Window::StartRendering()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::StopRendering()
{
	/* Swap front and back buffers */
	glfwSwapBuffers(window);

	/* Poll for and process events */
	glfwPollEvents();
}

void Window::DrawImage(string textureName, glm::vec2 position, glm::vec2 scale, float rotation, glm::vec3 color)
{
	shader->use();
	Texture& texture = textures[textureName];
	glm::vec2 newTextSize{ scale.x * texture.Width, scale.y * texture.Height };
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * newTextSize.x, 0.5f * newTextSize.y, 0.0f)); // move origin of rotation to center of quad
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
	model = glm::translate(model, glm::vec3(-0.5f * newTextSize.x, -0.5f * newTextSize.y, 0.0f)); // move origin back

	model = glm::scale(model, glm::vec3(newTextSize, 1.0f)); // last scale

	shader->setMatrix4("model", model);

	// render textured quad
	shader->setVector3f("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

Window::Window()
{
	window = glfwCreateWindow(WindowWidth, WindowHeight, "Tamagocub", nullptr, nullptr);
}

Window::~Window()
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

#include "Window.h"
#include "Shader.h"
#include "TextRenderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <filesystem>
#include <glm.hpp>
#include "Texture.h"
#include "AnimatedTexture.h"

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

	glGenVertexArrays(1, &staticVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(staticVAO);
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
	return textures[name];
}

AnimatedTexture& Window::LoadAnimatedTexture(const char* file, std::string name, unsigned spriteWidth, unsigned spriteHeight, float framePerSec)
{
	AnimatedTexture animTexture;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	if (data)
	{
		// now generate texture
		animTexture.Generate(width, height, spriteWidth, spriteHeight, framePerSec, data);
	}
	else
	{
		cout << "Error loading texture : \"" << file << "\"" << endl;
	}
	stbi_image_free(data);
	animatedTextures[name] = animTexture;
	return animatedTextures[name];
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

	LoadAnimatedTexture("Resources\\Animated\\FeedAnimated.png", "feedAnim", 240, 240, 10);
	LoadAnimatedTexture("Resources\\Animated\\GiftAnimated.png", "giftAnim", 238, 213, 5);
	LoadAnimatedTexture("Resources\\Animated\\IdleAnimated.png", "idleAnim", 240, 240, 6);
	LoadAnimatedTexture("Resources\\Animated\\SportAnimated.png", "sportAnim", 240, 240, 10);
	LoadAnimatedTexture("Resources\\Animated\\HungryAnimated.png", "hungryAnim", 240, 208, 5);
	LoadAnimatedTexture("Resources\\Animated\\WetAnimated.png", "wetAnim", 240, 240, 5);
	LoadAnimatedTexture("Resources\\Animated\\HappyBirthdayAnimated.png", "birthdayAnim", 240, 212, 5);
	LoadAnimatedTexture("Resources\\Animated\\HealAnimated.png", "healAnim", 240, 240, 5);
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

void Window::DrawImage(string textureName, glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color, bool keepRatio)
{
	shader->use();
	Texture& texture = textures[textureName];

	glm::vec2 actualSize(size);
	if (keepRatio)
	{
		float realRatio = (float)texture.Height / texture.Width;
		float wantedRatio = size.x / size.y;
		if (realRatio != wantedRatio)
		{
			if (size.x <= size.y)
			{
				actualSize.y = size.x / (size.y * texture.Height);
			}
			else
			{
				actualSize.x = texture.Width * size.x / size.y;
			}
		}
	}

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * actualSize.x, 0.5f * actualSize.y, 0.0f)); // move origin of rotation to center of quad
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
	model = glm::translate(model, glm::vec3(-0.5f * actualSize.x, -0.5f * actualSize.y, 0.0f)); // move origin back

	model = glm::scale(model, glm::vec3(actualSize, 1.0f)); // last scale

	shader->setMatrix4("model", model);

	// render textured quad
	shader->setVector3f("spriteColor", color);

	shader->setVector2f("size", glm::vec2(1.0f));
	shader->setVector2f("offsetInd", glm::vec2(0.0f));

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(staticVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Window::DrawAnimatedImage(float deltaTime, std::string textureName, glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color, bool keepRatio)
{
	shader->use();
	AnimatedTexture& animTexture = animatedTextures[textureName];

	glm::vec2 actualSize(size);
	if (keepRatio)
	{
		float realRatio = (float)animTexture.spriteSize.x / animTexture.spriteSize.y;
		float wantedRatio = size.x / size.y;
		if (realRatio != wantedRatio)
		{
			if (size.x <= size.y)
			{
				actualSize.y = size.y * animTexture.spriteSize.y / size.x;
			}
			else
			{
				actualSize.x = animTexture.spriteSize.x * size.x / size.y;
			}
		}
	}

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * actualSize.x, 0.5f * actualSize.y, 0.0f)); // move origin of rotation to center of quad
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
	model = glm::translate(model, glm::vec3(-0.5f * actualSize.x, -0.5f * actualSize.y, 0.0f)); // move origin back

	model = glm::scale(model, glm::vec3(actualSize, 1.0f)); // last scale

	shader->setMatrix4("model", model);

	// render textured quad
	shader->setVector3f("spriteColor", color);

	unsigned frameInd = animTexture.GetCurrentFrame();

	glm::vec2 sizeRatio((float)animTexture.spriteSize.x / animTexture.sheetSize.x, (float)animTexture.spriteSize.y / animTexture.sheetSize.y);
	shader->setVector2f("size", sizeRatio);

	glm::vec2 offset(frameInd, 0);
	shader->setVector2f("offsetInd", offset);


	glActiveTexture(GL_TEXTURE0);
	animTexture.Bind(deltaTime);

	glBindVertexArray(staticVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Window::ResetAnimation(std::string textureName)
{
	animatedTextures[textureName].Reset();
}

const AnimatedTexture& Window::GetAnimTexture(std::string name)
{
	return animatedTextures[name];
}


Window::Window()
{
	window = glfwCreateWindow(WindowWidth, WindowHeight, "Tamagocub", nullptr, nullptr);
}

Window::~Window()
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

#include "KeyInput.h"
#include "Window.h"
#include "./CoreGame/Tamagocub.h"
#include <iostream>

KeyInput* KeyInput::Instance = nullptr;

KeyInput* KeyInput::InitialiseInputs(Window* window, Tamagocub* tamagochi)
{
	if (!KeyInput::Instance)
	{
		KeyInput::Instance = new KeyInput(window, tamagochi);
	}
	glfwSetKeyCallback(window->window, KeyInput::key_callback);
	return KeyInput::Instance;
}

void KeyInput::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	KeyInput::Instance->keyCallback(key, scancode, action, mods);
}

KeyInput::KeyInput(Window* window, Tamagocub* tamagochi) : window(window), tamagocub(tamagochi)
{
}

void KeyInput::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F && action == GLFW_RELEASE)
	{
		tamagocub->Feed();
	}
	if (key == GLFW_KEY_H && action == GLFW_RELEASE)
	{
		tamagocub->Heal();
	}
	if (key == GLFW_KEY_C && action == GLFW_RELEASE)
	{
		tamagocub->Clean();
	}
	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
	{
		tamagocub->Dispute();
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	{
		tamagocub->DoSport();
	}
}

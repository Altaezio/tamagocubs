#pragma once

struct GLFWwindow;
class Window;
class Tamagocub;

class KeyInput
{
public:
	static KeyInput* InitialiseInputs(Window* window, Tamagocub* tamagochi);
private:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static KeyInput* Instance;

	KeyInput(Window* window, Tamagocub* tamagochi);

	Window* window;
	Tamagocub* tamagocub;

	void keyCallback(int key, int scancode, int action, int mods);
};

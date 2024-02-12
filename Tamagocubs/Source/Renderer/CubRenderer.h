#pragma once
#include "../Window.h"

class Tamagocub;

class CubRenderer
{
private:
	Window* window;
	Tamagocub* tamagocub;

	void OnStateChanged();

public:
	CubRenderer(Window* window, Tamagocub* tamagocub);
};


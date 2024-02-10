#pragma once
#include "../Window.h"
#include "../CoreGame/Tamagocub.h"


class CubRenderer
{
private:
	Window* window;
	Tamagocub* tamagocub;

	void OnStateChanged();

public:
	CubRenderer(Window* window, Tamagocub* tamagocub);
};


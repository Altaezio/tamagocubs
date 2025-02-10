#pragma once
#include "Window.h"
#include "../Enums.h"

class Tamagocub;

class CubRenderer
{
public:
	CubRenderer(Window* window, Tamagocub* tamagocub);

	void Draw(float deltaTime);

private:
	Window* window;
	Tamagocub* tamagocub;

	std::string GetStateFileName(CubState state) const;
	std::string GetActionFileName(TamActions action) const;

	void OnActionExecuted(TamActions action);
	TamActions lastAction;
	const float actionShownTime = 3;
	bool actionTriggered;
	float actionTimeLeft;
};


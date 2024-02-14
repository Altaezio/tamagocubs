#pragma once
#include "../Window.h"
#include "../Enums.h"

class Tamagocub;

class CubRenderer
{
public:
	CubRenderer(Window* window, Tamagocub* tamagocub);

	void Update(float deltaTime);

private:
	Window* window;
	Tamagocub* tamagocub;

	void OnStateChanged();

	void OnFeedActionExecuted();
	void OnHealActionExecuted();
	void OnCleanActionExecuted();
	void OnDisputeActionExecuted();
	void OnDoSportActionExecuted();
	void OnActionExecuted(TamActions action);
	const float actionShownTime = 3;
	bool actionTriggered;
	float actionTimeLeft;
};


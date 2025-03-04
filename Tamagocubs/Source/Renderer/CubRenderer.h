#pragma once
#include <glm.hpp>
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

	std::string GetStateImageName(CubState state) const;
	std::string GetActionImageName(TamActions action) const;

	void OnStateChaged();

	void OnActionExecuted(TamActions action);
	TamActions lastAction;
	const float actionShownTime = 3;
	bool actionTriggered;
	float actionTimeLeft;

	void OnBirthday(unsigned newAge);
	bool partyBirthday = false;

	void ShowReactionMessage(glm::vec2 centeredPos);
};


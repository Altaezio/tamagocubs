#pragma once
#include <list>

class Event;

enum class CubState { idle, hungry, sick, wet, wontDo };

class Tamagocub
{

private:
	unsigned age;
	float ageInSeconds;
	const float secondsPerYear = 216; // 21600s = 6h

	float weight;
	const float weightGainPerFeeding = 2;
	const float weightLossPerGettingHungry = 1;
	const float weightLossPerDoingSport = 3;

	unsigned hunger;
	const unsigned maxHunger = 5;
	const float timeToGetOneHunger = 18; // 1800s = 30min
	const float timeHungerOffset = 6;
	float hungerCountDown;

	CubState currentState;
	const float cooldownBetweenChangedState = 6;
	bool justCameBackIdle;
	float timeSinceLastChanged;

	const float timeBetweenMoodChange = 27; // 2700 = 45min
	const float timeBetweenMoodChangeOffset = 9; // 900 = 15min
	float moodChangeCountDown;

	void HappyBirthday();
	void GettingHungry();
	void ChangeMood();

	void GoToNewState(CubState newState);

public:
	Tamagocub();

	void Update(float deltaTime);

	void Feed();
	void Heal();
	void Clean();
	void Dispute();
	void DoSport();

	CubState GetCurrentState() const;

	Event* StateChanged;
};

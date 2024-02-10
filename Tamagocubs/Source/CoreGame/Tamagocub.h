#pragma once
#include <list>
class Tamagocub
{
private:
	unsigned age;
	float ageInSeconds;
	const float secondsPerYear = 21600; // 6h

	float weight;
	const float weightGainPerFeeding = 2;
	const float weightLossPerGettingHungry = 1;
	const float weightLossPerDoingSport = 3;

	unsigned hunger;
	const unsigned maxHunger = 5;
	const float timeToGetOneHunger = 1800; // 30min
	const float timeHungerOffset = 600;
	float hungerCountDown;

	enum CubState { idle, hungry, sick, wet, wontDo };
	CubState currentState;
	const float cooldownBetweenChangedState = 60;
	bool justCameBackIdle;
	float timeSinceLastChanged;

	const float timeBetweenMoodChange = 2700; // 45min
	const float timeBetweenMoodChangeOffset = 900; // 15min
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

	std::list<void ()> StateChanged;
};

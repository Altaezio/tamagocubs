#pragma once
#include <list>
#include "../Enums.h"
#include "../Event.h"


class DataSaver;

class Tamagocub
{
public:
	Tamagocub();

	void Update(float deltaTime);

	void Feed();
	void Heal();
	void Clean();
	void Dispute();
	void DoSport();

	CubState GetCurrentState() const;

	Event<>* StateChanged;
	Event<>* FinishedIdling;
	Event<TamActions>* ActionExecuted;

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
	bool justCameBackIdle() const { return timeSinceLastChanged != 0; }
	float timeSinceLastChanged;

	const float timeBetweenMoodChange = 27; // 2700 = 45min
	const float timeBetweenMoodChangeOffset = 9; // 900 = 15min
	float moodChangeCountDown;

	void HappyBirthday();
	void GettingHungry();
	void ChangeMood();

	/// <summary>
	/// Change current state of the tamacub
	/// </summary>
	/// <param name="newState">the new state the tamacub should go to</param>
	/// <returns>True if the state actually changed</returns>
	bool GoToNewState(CubState newState);

	friend DataSaver;
};

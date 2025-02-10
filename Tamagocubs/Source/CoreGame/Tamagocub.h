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

	CubState GetCurrentState() const { return currentState; };
	double GetCurrentBlouze() const { return blouze; };
	double GetCurrentBlouzePerSec() const;

	Event<>* StateChanged;
	Event<>* FinishedIdling;
	Event<TamActions>* ActionExecuted;

private:
	unsigned age; // saved
	float ageInSeconds; // saved
	const float secondsPerYear = 216; // 21600s = 6h

	double blouze; // saved
	double blouzePerSecond; // saved

	float weight; // saved
	const float weightGainPerFeeding = 2;
	const float weightLossPerGettingHungry = 1;
	const float weightLossPerDoingSport = 3;

	unsigned hunger; // saved
	const unsigned maxHunger = 5;
	const float timeToGetOneHunger = 18; // 1800s = 30min
	const float timeHungerOffset = 6;
	float hungerCountDown; // saved

	CubState currentState; // saved
	const float cooldownBetweenChangedState = 6;
	bool justCameBackIdle() const { return timeSinceLastIdle < cooldownBetweenChangedState; }
	float timeSinceLastIdle; // saved

	const float timeBetweenMoodChange = 27; // 2700 = 45min
	const float timeBetweenMoodChangeOffset = 9; // 900 = 15min
	float moodChangeCountDown; // saved

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

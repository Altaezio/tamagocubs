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

	unsigned GetAge() const { return age; }
	double GetBlouze() const { return blouze; };
	double GetBPS() const;
	float GetWeight() const { return weight; }
	unsigned GetHunger()const { return hunger; }
	CubState GetState() const { return currentState; };

	Event<>* StateChanged;
	Event<>* FinishedIdling;
	Event<TamActions>* ActionExecuted;
	Event<unsigned>* Birthday;

	const float secondsPerYear = 21600; // = 6h (216)

	const float weightGainPerFeeding = 2;
	const float weightLossPerGettingHungry = 0.25;
	const float startWeight = 10.0f;
	const float minGoodWeight = 5.0f;
	const float maxGoodWeight = 15.0f;

	const unsigned maxHunger = 5;
	const float timeToGetOneHunger = 1800; // = 30min (18)
	const float timeHungerOffset = 900; // = 15min	(6)

	const float weightLossPerSport = 1;
	const float timeToGetHungryFracLostPerSport = 0.1f;

	const float cooldownBetweenChangedState = 6;

	const float timeBetweenMoodChange = 2700; // = 45min (27)
	const float timeBetweenMoodChangeOffset = 900; // = 15min (9)
private:
	unsigned age; // saved
	float ageInSeconds; // saved

	double blouze; // saved
	double blouzePerSecond; // saved

	float weight; // saved

	unsigned hunger; // saved
	float hungerCountDown; // saved

	CubState currentState; // saved
	float timeSinceLastIdle; // saved

	float moodChangeCountDown; // saved

	bool JustCameBackIdle() const { return timeSinceLastIdle < cooldownBetweenChangedState; }
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

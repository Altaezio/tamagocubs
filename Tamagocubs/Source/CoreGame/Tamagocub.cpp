#include "Tamagocub.h"
#include <stdlib.h>
#include "../CustomRandom.h"
#include "../Event.h"
#include <iostream>

using namespace std;

Tamagocub::Tamagocub() : age(0), ageInSeconds(0), weight(10), hunger(0), currentState(CubState::idle), timeSinceLastChanged(0)
{
	StateChanged = new Event();
	FinishedIdling = new Event();
	FeedActionExecuted = new Event();
	HealActionExecuted = new Event();
	CleanActionExecuted = new Event();
	DisputeActionExecuted = new Event();
	DoSportActionExecuted = new Event();
	int randomValue = rand();
	hungerCountDown = CustomRandom::RandomFloatOffset(timeToGetOneHunger, timeHungerOffset);
	moodChangeCountDown = CustomRandom::RandomFloatOffset(timeBetweenMoodChange, timeBetweenMoodChangeOffset);
}

void Tamagocub::Update(float deltaTime)
{
	ageInSeconds += deltaTime;
	if (ageInSeconds >= secondsPerYear)
	{
		HappyBirthday();
	}

	hungerCountDown -= deltaTime;
	if (hungerCountDown <= 0)
	{
		GettingHungry();
	}

	moodChangeCountDown -= deltaTime;
	if (moodChangeCountDown <= 0)
	{
		ChangeMood();
		moodChangeCountDown = CustomRandom::RandomFloatOffset(timeBetweenMoodChange, timeBetweenMoodChangeOffset);
	}

	if (justCameBackIdle())
	{
		timeSinceLastChanged += deltaTime;
		if (timeSinceLastChanged >= cooldownBetweenChangedState)
		{
			timeSinceLastChanged = 0;
			FinishedIdling->fire();

			cout << "Not just came back from idling anymore" << endl;
		}
	}
}

void Tamagocub::Feed()
{
	if (currentState == CubState::wontDo)
	{
		return;
	}

	weight += weightGainPerFeeding;
	FeedActionExecuted->fire();

	cout << "Eat your vegetables ! You're fater : " << weight << " but less hungry : " << hunger << endl;

	if (hunger == maxHunger)
	{
		GoToNewState(CubState::idle);
	}

	if (hunger > 0)
	{
		hunger--;
	}
}

void Tamagocub::Heal()
{
	if (currentState == CubState::sick)
	{
		if (GoToNewState(CubState::idle))
		{
			HealActionExecuted->fire();
			cout << "Here you are little bug." << endl;
		}
	}
}

void Tamagocub::Clean()
{
	if (currentState == CubState::wet)
	{
		if (GoToNewState(CubState::idle))
		{
			CleanActionExecuted->fire();
			cout << "Shiny like chrome" << endl;
		}
	}
}

void Tamagocub::Dispute()
{
	if (currentState == CubState::wontDo)
	{
		if (GoToNewState(CubState::idle))
		{
			DisputeActionExecuted->fire();
			cout << "OBEY ME" << endl;
		}
	}
}

void Tamagocub::DoSport()
{
	if (currentState == CubState::wontDo)
	{
		return;
	}

	weight = max(0.0f, weight - weightLossPerDoingSport);
	DoSportActionExecuted->fire();
	cout << "Litlle bit of sport won't hurt. You're less fat : " << weight << endl;
}

CubState Tamagocub::GetCurrentState() const
{
	return currentState;
}

void Tamagocub::HappyBirthday()
{
	age++;
	ageInSeconds -= secondsPerYear;
	cout << "Happy birthday !! Age : " << age << endl;
}

void Tamagocub::GettingHungry()
{
	weight -= max(0.0f, weight - weightLossPerGettingHungry);
	hunger = min(maxHunger, hunger + 1);
	if (hunger == maxHunger)
	{
		GoToNewState(CubState::hungry);
	}
	hungerCountDown = CustomRandom::RandomFloatOffset(timeToGetOneHunger, timeHungerOffset);
	cout << "Getting hungry : " << hunger << "/" << maxHunger << endl;
}

void Tamagocub::ChangeMood()
{
	if (currentState != CubState::idle)
	{
		return;
	}

	int randomint = CustomRandom::RandomInt(0, 3);
	CubState nextMood = CubState::sick;
	switch (randomint)
	{
	case 0:
		nextMood = CubState::sick;
		break;
	case 1:
		nextMood = CubState::wet;
		break;
	case 2:
		nextMood = CubState::wontDo;
		break;
	default:
		break;
	}
	GoToNewState(nextMood);
}

bool Tamagocub::GoToNewState(CubState newState)
{
	if (currentState == CubState::idle && justCameBackIdle() && hunger != maxHunger) // should only change from idle ?
	{
		return false;
	}

	if (currentState == CubState::hungry && newState == CubState::idle) // should happen only on feeding while hungry
	{
		hungerCountDown = CustomRandom::RandomFloatOffset(timeToGetOneHunger, timeHungerOffset);
	}

	currentState = newState;
	cout << "Mood changed : " << (int)currentState << endl;

	StateChanged->fire();

	if (newState == CubState::idle)
	{
		timeSinceLastChanged = 0;
	}
	return true;
}

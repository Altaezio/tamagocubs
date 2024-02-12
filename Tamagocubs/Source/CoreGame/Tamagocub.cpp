#include "Tamagocub.h"
#include <stdlib.h>
#include "../CustomRandom.h"
#include "../Event.h"
#include <iostream>

using namespace std;

void Tamagocub::HappyBirthday()
{
	age++;
	ageInSeconds -= secondsPerYear;
}

void Tamagocub::GettingHungry()
{
	weight -= weightLossPerGettingHungry;
	hunger++;
	if (hunger == maxHunger)
	{
		GoToNewState(CubState::hungry);
	}
	else
	{
		hungerCountDown = CustomRandom::RandomFloatOffset(timeToGetOneHunger, timeHungerOffset);
	}
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

void Tamagocub::GoToNewState(CubState newState)
{
	if (currentState == CubState::idle && justCameBackIdle && hunger != maxHunger) // should only change from idle ?
	{
		return;
	}

	if (currentState == CubState::hungry && newState == CubState::idle) // should happen only on feeding while hungry
	{
		hungerCountDown = CustomRandom::RandomFloatOffset(timeToGetOneHunger, timeHungerOffset);
	}

	currentState = newState;

	StateChanged->fire();

	if (newState == CubState::idle)
	{
		justCameBackIdle = true;
		timeSinceLastChanged = 0;
	}
}

Tamagocub::Tamagocub() : age(0), ageInSeconds(0), weight(10), hunger(0), currentState(CubState::idle), justCameBackIdle(true), timeSinceLastChanged(0)
{
	StateChanged = new Event();
	int randomValue = rand();
	hungerCountDown = CustomRandom::RandomFloatOffset(timeToGetOneHunger, timeHungerOffset);
	moodChangeCountDown = CustomRandom::RandomFloatOffset(timeBetweenMoodChange, timeBetweenMoodChangeOffset);
}

void Tamagocub::Update(float deltaTime)
{
	cout << "age : " << age << " hunger : " << hunger << " moodChangeCountDown : " << moodChangeCountDown << " deltatime : " << deltaTime << endl;
	
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

	if (justCameBackIdle)
	{
		timeSinceLastChanged += deltaTime;
		if (timeSinceLastChanged >= cooldownBetweenChangedState)
		{
			justCameBackIdle = false;
			timeSinceLastChanged = 0;
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
		GoToNewState(CubState::idle);
	}
}

void Tamagocub::Clean()
{
	if (currentState == CubState::wet)
	{
		GoToNewState(CubState::idle);
	}
}

void Tamagocub::Dispute()
{
	if (currentState == CubState::wontDo)
	{
		GoToNewState(CubState::idle);
	}
}

void Tamagocub::DoSport()
{
	if (currentState == CubState::wontDo)
	{
		return;
	}

	weight -= weightLossPerDoingSport;
}

CubState Tamagocub::GetCurrentState() const
{
	return currentState;
}

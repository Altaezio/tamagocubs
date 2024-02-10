#include "Tamagocub.h"
#include <stdlib.h>
#include "../CustomRandom.h"

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
		GoToNewState(hungry);
	}
	else
	{
		hungerCountDown = CustomRandom::RandomFloatOffset(timeToGetOneHunger, timeHungerOffset);
	}
}

void Tamagocub::ChangeMood()
{
	if (currentState == idle)
	{
		return;
	}

	int randomint = CustomRandom::RandomInt(0, 3);
	CubState nextMood = sick;
	switch (randomint)
	{
	case 0:
		nextMood = sick;
		break;
	case 1:
		nextMood = wet;
		break;
	case 2:
		nextMood = wontDo;
		break;
	default:
		break;
	}
	GoToNewState(nextMood);
	moodChangeCountDown = CustomRandom::RandomFloatOffset(timeBetweenMoodChange, timeBetweenMoodChangeOffset);
}

void Tamagocub::GoToNewState(CubState newState)
{
	if (currentState == idle && justCameBackIdle && hunger != maxHunger) // should only change from idle ?
	{
		return;
	}

	if (currentState == hungry && newState == idle) // should happen only on feeding while hungry
	{
		hungerCountDown = CustomRandom::RandomFloatOffset(timeToGetOneHunger, timeHungerOffset);
	}

	currentState = newState;

	for (auto callback : StateChanged)
	{
		callback();
	}

	if (newState == idle)
	{
		justCameBackIdle = true;
		timeSinceLastChanged = 0;
	}
}

Tamagocub::Tamagocub() : age(0), ageInSeconds(0), weight(10), hunger(0), currentState(idle), justCameBackIdle(true), timeSinceLastChanged(0)
{
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
	if (currentState == wontDo)
	{
		return;
	}

	weight += weightGainPerFeeding;
	if (hunger == maxHunger)
	{
		GoToNewState(idle);
	}

	if (hunger > 0)
	{
		hunger--;
	}
}

void Tamagocub::Heal()
{
	if (currentState == sick)
	{
		GoToNewState(idle);
	}
}

void Tamagocub::Clean()
{
	if (currentState == wet)
	{
		GoToNewState(idle);
	}
}

void Tamagocub::Dispute()
{
	if (currentState == wontDo)
	{
		GoToNewState(idle);
	}
}

void Tamagocub::DoSport()
{
	if (currentState == wontDo)
	{
		return;
	}

	weight -= weightLossPerDoingSport;
}

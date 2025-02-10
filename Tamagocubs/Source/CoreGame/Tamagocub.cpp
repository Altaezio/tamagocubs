#include "Tamagocub.h"
#include <stdlib.h>
#include "../CustomRandom.h"
#include <iostream>

using namespace std;

const double baseBlouzePerSecond = 0.1;

Tamagocub::Tamagocub() :
	age(0),
	ageInSeconds(0),
	blouze(0.0),
	blouzePerSecond(baseBlouzePerSecond),
	weight(10),
	hunger(0),
	currentState(CubState::idle),
	timeSinceLastIdle(0)
{
	StateChanged = new Event<>();
	FinishedIdling = new Event<>();
	ActionExecuted = new Event<TamActions>();
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

	blouze += GetCurrentBlouzePerSec() * deltaTime;

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
		timeSinceLastIdle += deltaTime;
		if (timeSinceLastIdle >= cooldownBetweenChangedState)
		{
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
	ActionExecuted->fire(TamActions::feed);

	if (hunger == maxHunger)
	{
		GoToNewState(CubState::idle);
	}

	if (hunger > 0)
	{
		hunger--;
	}

	cout << "Eat your vegetables ! You're fater : " << weight << " but less hungry : " << hunger << endl;
}

void Tamagocub::Heal()
{
	if (currentState == CubState::sick)
	{
		if (GoToNewState(CubState::idle))
		{
			ActionExecuted->fire(TamActions::heal);
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
			ActionExecuted->fire(TamActions::clean);
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
			ActionExecuted->fire(TamActions::dispute);
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
	ActionExecuted->fire(TamActions::doSport);
	cout << "Litlle bit of sport won't hurt. You're less fat : " << weight << endl;
}

double Tamagocub::GetCurrentBlouzePerSec() const
{
	if (currentState != CubState::idle)
	{
		return 0.0;
	}
	else
	{
		return blouzePerSecond;
	}
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
		timeSinceLastIdle = 0;
	}
	return true;
}

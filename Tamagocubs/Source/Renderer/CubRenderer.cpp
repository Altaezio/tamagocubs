#include "CubRenderer.h"
#include "../CoreGame/Tamagocub.h"
#include "../Event.h"
#include "../EventCallback.h"

using namespace std;

const string idleFile = "Idle.png";
const string hungryFile = "Hungry.png";
const string sickFile = "Sick.png";
const string wetFile = "Wet.png";
const string wontDoFile = "WontDo.png";
const string disputeFile = "Dispute.png";
const string fedFile = "Fed.png";
const string healFile = "Heal.png";
const string cleanFile = "Clean.png";
const string doSport = "Sport.png";

void CubRenderer::OnStateChanged()
{
	CubState currentState = tamagocub->GetCurrentState();
	string fileName;
	switch (currentState)
	{
	case CubState::hungry:
		fileName = hungryFile;
		break;
	case CubState::sick:
		fileName = sickFile;
		break;
	case CubState::wet:
		fileName = wetFile;
		break;
	case CubState::wontDo:
		fileName = wontDoFile;
		break;
	case CubState::idle:
		if(actionTriggered)
		{
			return;
		}
	default:
		fileName = idleFile;
		break;
	}
	window->SetNewMiddleTexture(fileName);
}

void CubRenderer::OnFeedActionExecuted()
{
	OnActionExecuted(TamActions::feed);
}

void CubRenderer::OnHealActionExecuted()
{
	OnActionExecuted(TamActions::heal);
}

void CubRenderer::OnCleanActionExecuted()
{
	OnActionExecuted(TamActions::clean);
}

void CubRenderer::OnDisputeActionExecuted()
{
	OnActionExecuted(TamActions::dispute);
}

void CubRenderer::OnDoSportActionExecuted()
{
	OnActionExecuted(TamActions::doSport);
}

void CubRenderer::OnActionExecuted(TamActions action)
{
	string fileName;
	switch (action)
	{
	case TamActions::feed:
		fileName = fedFile;
		break;
	case TamActions::heal:
		fileName = healFile;
		break;
	case TamActions::clean:
		fileName = cleanFile;
		break;
	case TamActions::dispute:
		fileName = disputeFile;
		break;
	case TamActions::doSport:
		fileName = doSport;
		break;
	default:
		fileName = idleFile;
		break;
	}
	window->SetNewMiddleTexture(fileName);
	actionTriggered = true;
}

CubRenderer::CubRenderer(Window* window, Tamagocub* tamagocub) : window(window), tamagocub(tamagocub), actionTriggered(false), actionTimeLeft(actionShownTime)
{
	IEventCallback* callbackState = new EventCallback(this, &CubRenderer::OnStateChanged);
	tamagocub->StateChanged->addListener(callbackState);
	IEventCallback* callbackFeed = new EventCallback(this, &CubRenderer::OnFeedActionExecuted);
	tamagocub->FeedActionExecuted->addListener(callbackFeed);
	IEventCallback* callbackHeal = new EventCallback(this, &CubRenderer::OnHealActionExecuted);
	tamagocub->HealActionExecuted->addListener(callbackHeal);
	IEventCallback* callbackClean = new EventCallback(this, &CubRenderer::OnCleanActionExecuted);
	tamagocub->CleanActionExecuted->addListener(callbackClean);
	IEventCallback* callbackDispute = new EventCallback(this, &CubRenderer::OnDisputeActionExecuted);
	tamagocub->DisputeActionExecuted->addListener(callbackDispute);
	IEventCallback* callbackDoSport = new EventCallback(this, &CubRenderer::OnDoSportActionExecuted);
	tamagocub->DoSportActionExecuted->addListener(callbackDoSport);
}

void CubRenderer::Update(float deltaTime)
{
	if (actionTriggered)
	{
		actionTimeLeft -= deltaTime;
		if (actionTimeLeft <= 0)
		{
			actionTriggered = false;
			OnStateChanged();
			actionTimeLeft = actionShownTime;
		}
	}
}

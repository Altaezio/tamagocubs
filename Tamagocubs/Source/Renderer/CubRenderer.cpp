#include "CubRenderer.h"
#include "../CoreGame/Tamagocub.h"
#include "../Event.h"
#include "../EventCallback.h"
#include "TextRenderer.h"

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


CubRenderer::CubRenderer(Window* window, Tamagocub* tamagocub) : window(window), tamagocub(tamagocub), actionTriggered(false), actionTimeLeft(actionShownTime)
{
	tamagocub->ActionExecuted->addListener(new EventCallback(this, &CubRenderer::OnActionExecuted));

	OnStateChanged();
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
	window->textRenderer->RenderText("Bonjour", 5.0f, 5.0f, 1.0f);
}
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
		if (actionTriggered)
		{
			return;
		}
		[[fallthrough]];
	default:
		fileName = idleFile;
		break;
	}
	window->SetNewMiddleTexture(fileName);
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

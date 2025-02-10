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


CubRenderer::CubRenderer(Window* window, Tamagocub* tamagocub) :
	window(window),
	tamagocub(tamagocub),
	actionTriggered(false),
	actionTimeLeft(actionShownTime),
	lastAction(TamActions::none)
{
	tamagocub->ActionExecuted->addListener(new EventCallback(this, &CubRenderer::OnActionExecuted));
}

void CubRenderer::Draw(float deltaTime)
{
	if (actionTriggered)
	{
		actionTimeLeft -= deltaTime;
		if (actionTimeLeft <= 0)
		{
			actionTriggered = false;
			actionTimeLeft = actionShownTime;
		}
	}
	CubState currentState = tamagocub->GetCurrentState();
	string cubImage = GetStateFileName(currentState);
	if (actionTriggered && lastAction != TamActions::none)
	{
		cubImage = GetActionFileName(lastAction);
	}
	window->DrawImage(cubImage, glm::vec2(1.0f));

	char blouze[20];
	snprintf(blouze, 20, "Blouze: %.0f (%+.1f/s)", tamagocub->GetCurrentBlouze(), tamagocub->GetCurrentBlouzePerSec());
	window->textRenderer->RenderText(blouze, 5.0f, 5.0f, 1.0f);
}

string CubRenderer::GetStateFileName(CubState state) const
{
	string fileName;
	switch (state)
	{
	case CubState::hungry:
		fileName = "hungry";
		break;
	case CubState::sick:
		fileName = "sick";
		break;
	case CubState::wet:
		fileName = "wet";
		break;
	case CubState::wontDo:
		fileName = "wontDo";
		break;
	case CubState::idle:
		[[fallthrough]];
	default:
		fileName = "idle";
		break;
	}
	return fileName;
}

void CubRenderer::OnActionExecuted(TamActions action)
{
	lastAction = action;
	actionTriggered = true;
}

string CubRenderer::GetActionFileName(TamActions action) const
{
	string fileName;
	switch (action)
	{
	case TamActions::feed:
		fileName = "fed";
		break;
	case TamActions::heal:
		fileName = "heal";
		break;
	case TamActions::clean:
		fileName = "clean";
		break;
	case TamActions::dispute:
		fileName = "dispute";
		break;
	case TamActions::doSport:
		fileName = "sport";
		break;
	default:
		fileName = "idle";
		break;
	}
	return fileName;
}

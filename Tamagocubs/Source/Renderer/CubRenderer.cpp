#include "CubRenderer.h"
#include <format>
#include "../CoreGame/Tamagocub.h"
#include "../Event.h"
#include "../EventCallback.h"
#include "TextRenderer.h"
#include "AnimatedTexture.h"

using namespace std;


CubRenderer::CubRenderer(Window* window, Tamagocub* tamagocub) :
	window(window),
	tamagocub(tamagocub),
	actionTriggered(false),
	actionTimeLeft(actionShownTime),
	lastAction(TamActions::none)
{
	tamagocub->StateChanged->addListener(new EventCallback(this, &CubRenderer::OnStateChaged));
	tamagocub->ActionExecuted->addListener(new EventCallback(this, &CubRenderer::OnActionExecuted));
	tamagocub->Birthday->addListener(new EventCallback(this, &CubRenderer::OnBirthday));
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
	CubState currentState = tamagocub->GetState();
	string cubImage = GetStateImageName(currentState);
	if (actionTriggered && lastAction != TamActions::none)
	{
		cubImage = GetActionImageName(lastAction);
	}
	glm::vec2 size(225);
	glm::vec2 pos(.5f * (window->WindowWidth - size.x), .5f * (window->WindowHeight - size.y));
	if (!partyBirthday)
	{
		if (cubImage.substr(cubImage.size() - 4) == "Anim")
		{
			window->DrawAnimatedImage(deltaTime, cubImage, pos, size, 0.0f, glm::vec3(1.0f), true);
		}
		else
		{
			window->DrawImage(cubImage, pos, size, 0.0f, glm::vec3(1.0f), true);
		}

		glm::vec2 reactionCenteredPos(0.5f * window->WindowWidth, 0.8f * window->WindowHeight);
		ShowReactionMessage(reactionCenteredPos);
	}
	else
	{
		const AnimatedTexture& birthdayTex = window->GetAnimTexture("birthdayAnim");
		if (birthdayTex.currentTime + deltaTime > birthdayTex.GetTotalFrame() * birthdayTex.secondsPerFrame)
		{
			partyBirthday = false;
		}
		else
		{
			window->DrawAnimatedImage(deltaTime, "birthdayAnim", pos, size, 0.0f, glm::vec3(1.0f), true);
			string hb = format("Happy {0:}th Birthday!", tamagocub->GetAge());
			glm::vec2 hbPos(0.5f * (window->WindowWidth - hb.size() * window->textRenderer->cSize.x), 0.65f * window->WindowHeight);
			window->textRenderer->RenderText(hb, hbPos);
		}
	}

	string blouze = format("Blouze: {0:.0f} ({1:+.2f}/s)", tamagocub->GetBlouze(), tamagocub->GetBPS());
	window->textRenderer->RenderText(blouze, .5f * (window->WindowWidth - blouze.size() * window->textRenderer->cSize.x), 5.0f);

	string sport = format("Do (S)port");
	glm::vec2 sportPos(10.0f, window->WindowHeight - window->textRenderer->cSize.x - 10.0f);
	window->textRenderer->RenderText(sport, sportPos);

	string weight = format("Weight: {0:}", tamagocub->GetWeight());
	glm::vec2 weightPos(sportPos.x, sportPos.y - window->textRenderer->cSize.y - 5.0f);
	window->textRenderer->RenderText(weight, weightPos);

	string feed = format("(F)eed");
	glm::vec2 feedPos(window->WindowWidth - feed.size() * window->textRenderer->cSize.x, window->WindowHeight - window->textRenderer->cSize.x - 10.0f);
	window->textRenderer->RenderText(feed, feedPos);

	string hunger = format("Hunger: {0:}/{1:}", tamagocub->GetHunger(), tamagocub->maxHunger);
	glm::vec2 hungerPos(window->WindowWidth - hunger.size() * window->textRenderer->cSize.x, feedPos.y - window->textRenderer->cSize.y - 5.0f);
	window->textRenderer->RenderText(hunger, hungerPos);
}

void CubRenderer::OnStateChaged()
{
	CubState currentState = tamagocub->GetState();
	window->ResetAnimation(GetStateImageName(currentState));
}

string CubRenderer::GetStateImageName(CubState state) const
{
	string imageName;
	switch (state)
	{
	case CubState::hungry:
		imageName = "hungryAnim";
		break;
	case CubState::sick:
		imageName = "sick";
		break;
	case CubState::wet:
		imageName = "wetAnim";
		break;
	case CubState::wontDo:
		imageName = "wontDo";
		break;
	case CubState::idle:
		[[fallthrough]];
	default:
		imageName = "idleAnim";
		break;
	}
	return imageName;
}

void CubRenderer::OnActionExecuted(TamActions action)
{
	lastAction = action;
	actionTriggered = true;
	window->ResetAnimation(GetActionImageName(action));
}

string CubRenderer::GetActionImageName(TamActions action) const
{
	string imageName;
	switch (action)
	{
	case TamActions::feed:
		imageName = "feedAnim";
		break;
	case TamActions::heal:
		imageName = "healAnim";
		break;
	case TamActions::clean:
		imageName = "clean";
		break;
	case TamActions::dispute:
		imageName = "dispute";
		break;
	case TamActions::doSport:
		imageName = "sportAnim";
		break;
	default:
		imageName = "idleAnim";
		break;
	}
	return imageName;
}

void CubRenderer::OnBirthday(unsigned newAge)
{
	partyBirthday = true;
	window->ResetAnimation("birthdayAnim");
}

void CubRenderer::ShowReactionMessage(glm::vec2 centeredPos)
{
	string text = "";
	switch (tamagocub->GetState())
	{
	case CubState::hungry:
		text = "Too hungry, (F)eed it";
		break;
	case CubState::sick:
		text = "Too sad, (H)ug it";
		break;
	case CubState::wet:
		text = "Too wet, (C)lean it";
		break;
	case CubState::wontDo:
		text = "Don't want to do, (D)ispute it";
		break;
	case CubState::idle:
		break;
	default:
		break;
	}
	if (text.size() == 0)
	{
		return;
	}
	glm::vec2 leftPos(centeredPos.x - 0.5f * text.size() * window->textRenderer->cSize.x, centeredPos.y);
	window->textRenderer->RenderText(text, leftPos);
}

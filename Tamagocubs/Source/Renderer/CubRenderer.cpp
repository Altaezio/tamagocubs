#include "CubRenderer.h"
#include "../CoreGame/Tamagocub.h"
#include "../Event.h"
#include "../EventCallback.h"

using namespace std;

void CubRenderer::OnStateChanged()
{
	string fileName;
	switch (tamagocub->GetCurrentState())
	{
	case CubState::hungry:
		fileName = "Hungry.png";
		break;
	case CubState::sick:
		fileName = "Sick.png";
		break;
	case CubState::wet:
		fileName = "Wet.png";
		break;
	case CubState::wontDo:
		fileName = "WontDo.png";
		break;
	case CubState::idle:
	default:
		fileName = "Idle.png";
		break;
	}
	window->SetNewMiddleTexture(fileName);
}

CubRenderer::CubRenderer(Window* window, Tamagocub* tamagocub) : window(window), tamagocub(tamagocub)
{
	IEventCallback* callback = new EventCallback(this, &CubRenderer::OnStateChanged);
	tamagocub->StateChanged->addListener(callback);
}

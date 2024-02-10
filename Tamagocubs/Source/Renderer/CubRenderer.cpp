#include "CubRenderer.h"

using namespace std;

void CubRenderer::OnStateChanged()
{
	cout << "bite" << endl;
}

CubRenderer::CubRenderer(Window* window, Tamagocub* tamagocub) : window(window), tamagocub(tamagocub)
{
	tamagocub->StateChanged.push_back(OnStateChanged);
}

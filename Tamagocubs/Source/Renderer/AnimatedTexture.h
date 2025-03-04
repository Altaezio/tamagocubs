#pragma once
#include <glm.hpp>

class AnimatedTexture
{
public:
	unsigned ID;
	glm::uvec2 sheetSize;
	glm::uvec2 spriteSize;

	float secondsPerFrame;
	float currentTime;

	AnimatedTexture();
	void Generate(unsigned sheetWidth, unsigned sheetHeight, unsigned spriteWidth, unsigned spriteHeight, float framePerSeconds, unsigned char* data);
	void Bind(float deltaTime);

	unsigned GetTotalFrame() const { return sheetSize.x / (spriteSize.x + 1); }
	unsigned GetCurrentFrame() const { return (unsigned)(currentTime / secondsPerFrame) % GetTotalFrame(); }

	void Reset();
};
#include "AnimatedTexture.h"
#include <glad/glad.h>


AnimatedTexture::AnimatedTexture() : sheetSize(0), spriteSize(0), secondsPerFrame(0.0f), currentTime(0.0f)
{
	glGenTextures(1, &ID);
}

void AnimatedTexture::Generate(unsigned sheetWidth, unsigned sheetHeight, unsigned spriteWidth, unsigned spriteHeight, float framePerSecond, unsigned char* data)
{
	sheetSize.x = sheetWidth;
	sheetSize.y = sheetHeight;
	spriteSize.x = spriteWidth;
	spriteSize.y = spriteHeight;
	secondsPerFrame = 1.0f / framePerSecond;

	// create Texture
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sheetSize.x, sheetSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void AnimatedTexture::Bind(float deltaTime)
{
	currentTime += deltaTime;
	glBindTexture(GL_TEXTURE_2D, ID);
}

void AnimatedTexture::Reset()
{
	currentTime = 0.0f;
}
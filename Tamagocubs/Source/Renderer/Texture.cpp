#include "Texture.h"

Texture::Texture() : Width(0), Height(0)
{
	glGenTextures(1, &ID);
}

void Texture::Generate(unsigned width, unsigned height, unsigned char* data)
{
	Width = width;
	Height = height;
	// create Texture
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, ID);
}
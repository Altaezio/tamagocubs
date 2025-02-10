#pragma once

#include <glad/glad.h>

class Texture
{
public:
	unsigned ID;
	unsigned Width, Height;

	Texture();
	void Generate(unsigned width, unsigned height, unsigned char* data);
	void Bind() const;
};
#include "CustomRandom.h"
#include <chrono>

void CustomRandom::SetSeed()
{
	std::srand(std::time(nullptr));
}

int CustomRandom::RandomInt(int min, int max)
{
	return ((max - min) * (rand() / RAND_MAX)) + min;
}

float CustomRandom::RandomFloat(float min, float max)
{
	return ((max - min) * ((float)rand() / RAND_MAX)) + min;
}

int CustomRandom::RandomIntOffset(int middle, int offset)
{
	return RandomInt(middle - offset, middle + offset);
}

float CustomRandom::RandomFloatOffset(float middle, float offset)
{
	return RandomFloat(middle - offset, middle + offset);
}

#pragma once
class CustomRandom
{
private:
	int seed;
public:
	static void SetSeed();

	/// <summary>
	/// Returns a random int
	/// </summary>
	/// <param name="min">Minimum value output</param>
	/// <param name="max">Excluded max output</param>
	/// <returns>A random int between min and max excluded</returns>
	static int RandomInt(int min, int max);

	/// <summary>
	/// Returns a random float
	/// </summary>
	/// <param name="min">Minimum value output</param>
	/// <param name="max">Max output</param>
	/// <returns>A random int between min and max</returns>
	static float RandomFloat(float min, float max);

	static int RandomIntOffset(int middle, int offset);
	static float RandomFloatOffset(float middle, float offset);
};


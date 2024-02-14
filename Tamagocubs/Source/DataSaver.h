#pragma once
#include <filesystem>

class Tamagocub;

class DataSaver
{
public:
	static Tamagocub* LoadOrCreateTamagocub();

	static void SaveProgress(Tamagocub*);

private:
	static bool CheckOrCreateGameDirectoryExists(std::filesystem::path* outDirectorySavePath);
	static Tamagocub* ParseFileToTamagocub(std::filesystem::path pathToSaveFile);
};
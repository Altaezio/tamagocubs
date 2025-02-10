#include "DataSaver.h"
#include "./CoreGame/Tamagocub.h"
#include "Event.h"
#include "Enums.h"
#include <string>
#include <shlobj.h>
#include <iostream>
#include <fstream>

using namespace std;

const wstring saveDirectoryInAppdata = L"\\Tamagocub\\Save";
const wstring saveFileName = L"save.tc";
const wstring backupFileName = L"backup.tc";

Tamagocub* DataSaver::LoadOrCreateTamagocub()
{
	filesystem::path directorySavePath;
	if (!CheckOrCreateGameDirectoryExists(&directorySavePath))
	{
		return nullptr;
	}
	const filesystem::path saveFilePath{ directorySavePath / saveFileName };
	if (!filesystem::exists(saveFilePath))
	{
		return new Tamagocub();
	}

	return ParseFileToTamagocub(directorySavePath / saveFileName);
}

void DataSaver::SaveProgress(Tamagocub* tamagocub)
{
	filesystem::path directorySavePath;
	CheckOrCreateGameDirectoryExists(&directorySavePath);

	const filesystem::path saveFilePath{ directorySavePath / saveFileName };
	if (filesystem::exists(saveFilePath))
	{
		rename(saveFilePath, directorySavePath / backupFileName);
	}
	ofstream saveFile{ saveFilePath };

	if (saveFile.is_open())
	{
		saveFile << "v:25" << endl;
		saveFile << "age:" << tamagocub->age << endl;
		saveFile << "ageSeconds:" << tamagocub->ageInSeconds << endl;
		saveFile << "blouze:" << tamagocub->blouze << endl;
		saveFile << "blouzePerSec:" << tamagocub->blouzePerSecond << endl;
		saveFile << "weight:" << tamagocub->weight << endl;
		saveFile << "hunger:" << tamagocub->hunger << endl;
		saveFile << "hungerCountDown:" << tamagocub->hungerCountDown << endl;
		saveFile << "state:" << (int)tamagocub->currentState << endl;
		saveFile << "timeSinceLastIdle:" << tamagocub->timeSinceLastIdle << endl;
		saveFile << "moodChangeCountDown:" << tamagocub->moodChangeCountDown << endl;

		saveFile.close();
	}
	else { cout << "ERROR : unable to open file" << endl; }
}

bool DataSaver::CheckOrCreateGameDirectoryExists(filesystem::path* outDirectorySavePath)
{
	PWSTR localAppdata = nullptr;
	HRESULT getResult = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &localAppdata);
	if (getResult == E_FAIL)
	{
		cout << "ERROR to get local appdata : E_FAIL" << endl;
		return false;
	}
	else if (getResult == E_INVALIDARG)
	{

		cout << "ERROR to get local appdata : E_INVALIDARG" << endl;
		return false;
	}

	wstring stringLocalAppdata = wstring(localAppdata) + saveDirectoryInAppdata;
	const filesystem::path directorySavePath{ stringLocalAppdata };
	if (!filesystem::exists(directorySavePath))
	{
		filesystem::create_directories(directorySavePath);
	}
	*outDirectorySavePath = directorySavePath;
	return true;
}

Tamagocub* DataSaver::ParseFileToTamagocub(filesystem::path pathToSaveFile)
{
	Tamagocub* tamagocub = new Tamagocub();
	ifstream saveFile{ pathToSaveFile };

	if (saveFile.is_open())
	{
		unsigned version = 24;
		string line;
		getline(saveFile, line); // version
		if (line.substr(0, 1) == "v")
		{
			version = stoi(line.substr(2));
			getline(saveFile, line);
		}
		tamagocub->age = stoi(line.substr(4));
		getline(saveFile, line);
		tamagocub->ageInSeconds = stof(line.substr(11));
		if (version >= 25)
		{
			getline(saveFile, line);
			tamagocub->blouze = stod(line.substr(7));
			getline(saveFile, line);
			tamagocub->blouzePerSecond = stod(line.substr(13));
		}
		getline(saveFile, line);
		tamagocub->weight = stof(line.substr(7));
		getline(saveFile, line);
		tamagocub->hunger = stoi(line.substr(7));
		getline(saveFile, line);
		tamagocub->hungerCountDown = stof(line.substr(16));
		getline(saveFile, line);
		int stateInt = stoi(line.substr(6));
		CubState state = (CubState)stateInt;
		tamagocub->currentState = state;
		tamagocub->StateChanged->fire();
		getline(saveFile, line);
		if (version < 25)
			tamagocub->timeSinceLastIdle = stof(line.substr(21));
		else
			tamagocub->timeSinceLastIdle = stof(line.substr(18));

		getline(saveFile, line);
		tamagocub->moodChangeCountDown = stof(line.substr(20));

		saveFile.close();
	}
	else { cout << "ERROR : unable to open file" << endl; return nullptr; }

	return tamagocub;
}

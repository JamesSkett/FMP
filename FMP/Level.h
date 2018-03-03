#pragma once
#include <string>
#include <vector>
#include "Tile.h"

using namespace std;

class Level
{
public:
	Level();
	~Level();

	void LoadLevelData(string filePath);

private:
	vector <string> m_vlevelData;
	vector <Tile> m_vWalls;
	vector <Tile> m_vFloor;
};


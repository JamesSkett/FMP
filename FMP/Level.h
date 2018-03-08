#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>
#include "Tile.h"
#include "Player.h"
#include "Colours.h"

using namespace std;
using namespace DirectX;

class Level
{
public:
	Level();
	~Level();

	void LoadLevelData(string filePath);
	void SetUpLevelLayout(vector <Tile*> &vWalls, vector <Tile*> &vFloor, Player* &player);
	//void Draw(XMMATRIX view, XMMATRIX projection);

private:
	vector <string> m_vlevelData;
	
	Colour colour;

	float m_tileOffset = 2.25f;
};


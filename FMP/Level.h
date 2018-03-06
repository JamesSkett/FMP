#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>
#include "Tile.h"
#include "Colours.h"

using namespace std;
using namespace DirectX;

class Level
{
public:
	Level();
	~Level();

	void LoadLevelData(string filePath);
	void SetUpLevelLayout();
	void Draw(XMMATRIX view, XMMATRIX projection);

private:
	vector <string> m_vlevelData;
	vector <Tile*> m_vWalls;
	vector <Tile*> m_vFloor;

	Colour colour;
};


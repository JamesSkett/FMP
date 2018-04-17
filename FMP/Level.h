#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>

#include "Tile.h"
#include "Player.h"
#include "Monster.h"
#include "Projectile.h"


using namespace std;
using namespace DirectX;
class Asset;

class Level
{
public:
	Level();
	~Level();

	void LoadLevelData(string filePath);
	void SetUpLevelLayout(vector <Tile*> &tilemap, Player* &player, Monster* &monster, vector <Asset*> &vDoors);
	void LoadProjectiles(vector <Projectile*> &projectiles);

private:
	vector <string> m_vlevelData;
	

	float m_tileOffset = 2.25f;
};


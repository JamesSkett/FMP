/*
	Class:		Level
	Project:	Adaptive Monster AI for a Horror Game (Final Major Project)
	Author:		James Skett
	Purpose:	This loads in the level data from a text file then sets up the level placing tiles in the order it is in the text file
*/

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

class Level
{
public:
	Level();
	~Level();

	void LoadLevelData(const char* filePath);
	void SetUpLevelLayout(vector <Tile*> &tilemap, Player* &player, Monster* &monster);
	void LoadProjectiles(vector <Projectile*> &projectiles);

private:
	vector <string> m_vlevelData;
	

	float m_tileOffset = 2.25f;
};


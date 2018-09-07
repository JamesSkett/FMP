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



class Tile;
class Player;
class Monster;

using namespace DirectX;

class Level
{
public:
	Level();
	~Level();

	//load the level data from a text file
	void LoadLevelData(const char* filePath);
	//create and position the tiles 
	void SetUpLevelLayout(std::vector <Tile*> &tilemap, Player* &player, Monster* &monster);

private:
	std::vector <std::string> m_vlevelData;
	

	float m_tileOffset = 2.25f;
};


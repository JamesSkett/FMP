/*
	Class:		Player
	Parent:		Entity
	Project:	Adaptive Monster AI for a Horror Game (Final Major Project)
	Author:		James Skett
	Purpose:	Controls all the player actions like movement and used the CollisionCheck()
				function from its parent class Entity
*/

#pragma once

#include "Entity.h"

#include <vector>

using namespace std;

class Tile;

class Player : public Entity
{
public:
	Player(float x, float y, float z, float scale, float width, float height, float depth);
	~Player();

	void Update(XMFLOAT2 enemyPos, vector <Tile*> tilemap, float deltaTime);

	//Get the value of m_enemyInSight
	bool GetEnemyInSight();

	//Increases the player speed
	void SprintOn();
	//Resets the player speed to normal
	void SprintOff();

	//Checks to see if the player is in sight
	bool LineOfSightCheck(XMFLOAT2 targetPos, vector <Tile*> tilemap);

	//Checks if the tile at 'pos' is walkable
	bool CheckTile(XMFLOAT2 pos, vector <Tile*> tilemap);

	static float s_rotation;
	static XMFLOAT2 s_playerPos;

private:
	//Player move speed
	float m_velocity = 10.0f;

	bool m_enemyInSight = false;

	float m_timer = 0.02f;

	const float c_viewDistance = 3.0f;
};


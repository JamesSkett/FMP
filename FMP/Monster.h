#pragma once
#include "Entity.h"
#include "Tile.h"
#include "Pathfinding.h"
#include "Asset.h"

#include <vector>

using namespace std;

class Monster : public Entity
{
public:
	Monster(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height);
	~Monster();

	//Updates the monster 
	void Update(XMFLOAT2 targetPos, float deltaTime);


	//Gets the current x position value
	float GetXPos();
	//Gets the current y position value
	float GetYPos();
	//Get the value of m_playerInSight
	bool GetPlayerInSight();
	//Get the value of m_isSearching
	bool GetIsSearching();

	float GetRotation();

	//Moves to a specified point
	bool MoveTo(float x, float y, float deltaTime);

	//Creates the pathfinder object and gives it the tilemap
	void SetPathfinder(vector <Tile*> tilemap);
	
	bool LineOfSightCheck(XMFLOAT2 targetPos);

	//Behaviours
	void RandomWander(float deltaTime);
	void Chase(float deltaTime);
	void Search(XMFLOAT2 playerPos, float deltaTime);

	bool CheckTile(XMFLOAT2 pos);


private:
	float m_speed = 6.0f;
	Pathfinding* pathfinder;


	vector <XMFLOAT2> waypoints;

	vector <Tile*> m_tileMap;

	unsigned int waypointNum = 0;

	int num = 0;

	bool m_playerInSight = false;
	bool m_isSearching = false;

	float m_timer = 3.0f;

	XMFLOAT2 lastPlayerPos;
};

enum State
{
	RANDOM_WANDER = 0,
	CHASE = 1,
	SEARCH = 2,
	SNEAK = 3,
	FLEE = 4,
	ATTACK = 5
};
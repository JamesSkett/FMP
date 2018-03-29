#pragma once
#include "Entity.h"
#include "Tile.h"
#include "Pathfinding.h"

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

	//Moves to a specified point
	bool MoveTo(float x, float y, float deltaTime);

	//Creates the pathfinder object and gives it the tilemap
	void SetPathfinder(vector <Tile*> tilemap);
	
	bool LineOfSightCheck(XMFLOAT2 targetPos);

	//Behaviours
	void RandomWander(float deltaTime);

private:
	float m_speed = 10.0f;
	Pathfinding* pathfinder;

	vector <XMFLOAT2> waypoints;

	vector <Tile*> m_tileMap;

	unsigned int waypointNum = 0;
};


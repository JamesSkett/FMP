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

	void Update(XMFLOAT2 targetPos);

	float GetXPos();
	float GetYPos();

	bool MoveTo(float x, float y);

	void SetPathfinder(vector <Tile*> tilemap);
	
	bool LineOfSightCheck(XMFLOAT2 targetPos);

	//Behaviours
	void RandomWander();

private:
	float m_speed = 0.01f;
	Pathfinding* pathfinder;

	vector <XMFLOAT2> waypoints;

	vector <Tile*> m_tileMap;

	int waypointNum = 0;
};


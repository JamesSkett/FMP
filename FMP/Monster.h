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

private:
	float m_speed = 0.05f;
	Pathfinding* pathfinder;

	vector <XMFLOAT2> waypoints;

	int waypointNum = 0;
};


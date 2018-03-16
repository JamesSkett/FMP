#pragma once
#include "Tile.h"
#include "Monster.h"
#include "Player.h"

#include <vector>

using namespace std;

class Pathfinding
{
public:
	Pathfinding(vector <Tile*> tilemap);
	~Pathfinding();

	void UpdatePath(Monster* monster, XMFLOAT2 targetPos);

	void AddToOpenList(Tile* tile);
	void AddToClosedList(Tile* tile);

	Tile* GetStartTile(Monster* monster);
	bool IsAdjacent(Monster* monster, Tile* tile);

	void CalculateHValue(XMFLOAT2 targetPos);
	void FindAdjacentTiles(Monster* monster);

private:
	vector <Tile*> m_openList;
	vector <Tile*> m_closedList;
	vector <Tile*> m_tileMap;

	Tile* m_StartTile;
	
	bool m_pathFound;
};


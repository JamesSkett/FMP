#include "Pathfinding.h"
#include <math.h>


Pathfinding::Pathfinding(vector <Tile*> tilemap)
{
	m_tileMap = tilemap;
}


Pathfinding::~Pathfinding()
{
}

void Pathfinding::UpdatePath(Monster* monster, XMFLOAT2 targetPos)
{
	if (!m_pathFound)
	{
		AddToClosedList(GetStartTile(monster));

		for (int i = 0; i < m_tileMap.size(); i++)
		{
			for (int j = 0; j < m_closedList.size(); j++)
			{
				if (m_tileMap[i] == m_closedList[j])
				{
					break;
				}

				for (int x; x < m_openList.size(); x++)
				{
					if (m_tileMap[i] == m_openList[i])
					{
						break;
					}
					else
					{
						if (IsAdjacent(monster, m_tileMap[i]))
						{
							AddToOpenList(m_tileMap[i]);
						}
					}
				}

				break;
			}
		}
	}
}

void Pathfinding::AddToOpenList(Tile * tile)
{
	m_openList.push_back(tile);
}

void Pathfinding::AddToClosedList(Tile * tile)
{
	m_closedList.push_back(tile);
}

Tile* Pathfinding::GetStartTile(Monster * monster)
{
	float currentDistance;
	float minDistance = pow(m_tileMap[0]->GetXPos() - monster->GetXPos(), 2) + pow(m_tileMap[0]->GetYPos() - monster->GetYPos(), 2);
	for (int i = 0; i < m_tileMap.size(); i++)
	{
		float monsterPosX = monster->GetXPos();
		float monsterPosY = monster->GetYPos();
		float tilePosX = m_tileMap[i]->GetXPos();
		float tilePosY = m_tileMap[i]->GetYPos();

		currentDistance = ((tilePosX - monsterPosX) * (tilePosX - monsterPosX)) + ((tilePosY - monsterPosY) * (tilePosY - monsterPosY));

		if (currentDistance < minDistance)
		{
			minDistance = currentDistance;
			m_StartTile = m_tileMap[i];
		}
	}

	return m_StartTile;
}

bool Pathfinding::IsAdjacent(Monster* monster, Tile * tile)
{
	float monsterX = monster->GetXPos();
	float monsterY = monster->GetYPos();
	float tileX = tile->GetXPos();
	float tileY = tile->GetYPos();

	float distance = ((tileX - monsterX) * (tileX - monsterY)) + ((tileY - monsterY) * (tileY - monsterY));

	if (distance < 0.4f)
	{
		if (tile->GetIsWalkable())
		{
			AddToClosedList(tile);
			return false;
		}
		return true;
	}

	return false;
}
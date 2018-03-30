#include "Monster.h"
#include "GameSystem.h"
#include <time.h>


Monster::Monster(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height) :
	Entity(colour, x, y, z, scale, width, height)
{
	
}


Monster::~Monster()
{

}

void Monster::Update(XMFLOAT2 targetPos, float deltaTime)
{

	if (pathfinder->GetIsPathFound())
	{
		if (waypointNum >= waypoints.size())
		{
			pathfinder->SetIsPathFound(false);
			waypointNum = 0;
		}
		else if (MoveTo(waypoints[waypointNum].x, waypoints[waypointNum].y, deltaTime))
		{
			waypointNum++;
		}
	}
	else
	{
		waypoints = pathfinder->FindPath(XMFLOAT2(m_xPos, m_yPos), targetPos);
	}
	
}

float Monster::GetXPos()
{
	return m_xPos;
}

float Monster::GetYPos()
{
	return m_yPos;
}

bool Monster::MoveTo(float x, float y, float deltaTime)
{

	m_dirX = x - m_xPos;
	m_dirY = y - m_yPos;

	float distance = sqrt(m_dirX * m_dirX + m_dirY * m_dirY);

	if (distance < 0.1) return true;

	m_dirX /= distance;
	m_dirY /= distance;

	m_xPos += (m_dirX * m_speed) * deltaTime;
	m_yPos += (m_dirY * m_speed) * deltaTime;

	return false;
}

void Monster::SetPathfinder(vector<Tile*> tilemap)
{
	pathfinder = new Pathfinding(tilemap);
	m_tileMap = tilemap;
}

bool Monster::LineOfSightCheck(XMFLOAT2 targetPos, vector <Tile*> tilemap)
{
	XMFLOAT2 point = { 10, 10 };
	float angle;
	float directionX, directionY;
	float distance;

	angle = atan2f((targetPos.x - m_xPos), (targetPos.y - m_yPos));
	directionX = sinf(angle);
	directionY = cosf(angle);

	point = { m_xPos + (directionX * 0.2f), m_yPos + (directionY * 0.2f) };

	while (true)
	{
		point = { point.x + (directionX * 0.2f), point.y + (directionY * 0.2f) };

		if (point.x == targetPos.x && point.y == targetPos.y)
		{
			return true;
		}

		for (unsigned int i = 0; i < tilemap.size(); i++)
		{
			if (tilemap[i]->GetIsWalkable())
			{
				float tileX;
				float tileY;
				float tileW;
				float tileH;

				float pointW = 0.05f;
				float pointH = 0.05f;
				float pointX = point.x - (pointW / 2);
				float pointY = point.y - (pointH / 2);

				tilemap[i]->GetParameters(tileX, tileY, tileW, tileH);

				tileX = tileX - (tileW / 2);
				tileY = tileY - (tileH / 2);

				if ((pointX < tileX + tileW) && (pointX + pointW > tileX) && (pointY < tileY + tileH) && (pointY + pointH > tileY))
				{
					return false;
				}
			}
		}
		
	}
	

	return false;
}

//bool Monster::LineOfSightCheck(XMFLOAT2 targetPos, vector <Tile*> tilemap)
//{
//
//	for (unsigned int i = 0; i < tilemap.size(); i++)
//	{
//		float tileX;
//		float tileY;
//		float tileW;
//		float tileH;
//
//		XMFLOAT2 origin = { m_xPos, m_yPos };
//		XMFLOAT2 direction = { targetPos.x - m_xPos, targetPos.y - m_yPos };
//
//
//		tilemap[i]->GetParameters(tileX, tileY, tileW, tileH);
//
//		if (tilemap[i]->GetIsWalkable)
//		{
//			XMFLOAT2 boxMin = { tileX - (tileW / 2), tileY - (tileH / 2) };
//			XMFLOAT2 boxMax = { tileX + (tileW / 2), tileY + (tileH / 2) };
//
//			XMFLOAT2 tMin = { ((boxMin.x - origin.x) / direction.x), ((boxMin.y - origin.y) / direction.y) };
//			XMFLOAT2 tMax = { ((boxMax.x - origin.x) / direction.x), ((boxMax.y - origin.y) / direction.y) };
//
//			min(tMin, tMax);
//		}
//	}
//
//	return false;
//}

void Monster::RandomWander(float deltaTime)
{
	int randTileNum = rand() % 901;

	if (pathfinder->GetIsPathFound())
	{
		if (waypointNum >= waypoints.size())
		{
			pathfinder->SetIsPathFound(false);
			waypointNum = 0;
		}
		else if (MoveTo(waypoints[waypointNum].x, waypoints[waypointNum].y, deltaTime))
		{
			waypointNum++;
		}
	}
	else
	{
		waypoints = pathfinder->FindPath(XMFLOAT2(m_xPos, m_yPos), XMFLOAT2(m_tileMap[randTileNum]->GetXPos(), m_tileMap[randTileNum]->GetYPos()));
	}
	
}

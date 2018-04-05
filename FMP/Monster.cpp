#include "Monster.h"
#include "GameSystem.h"
#include <time.h>


Monster::Monster(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height) :
	Entity(colour, x, y, z, scale, width, height)
{
	m_rotation = 0;
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

bool Monster::LineOfSightCheck(XMFLOAT2 targetPos)
{
	// Make sure A is facing towards B

	//// Get angle we'd need to look straight at B
	//float angle = atan2f(targetPos.x - m_xPos, targetPos.y - m_yPos);
	//angle = angle * 180 / XM_PI;
	//// Keep rotation in a nice 360 circle
	//if (angle <= -1) angle = 360 + angle;
	//if (angle >= 360) angle = angle - 360;
	//// Now check if our rotation falls in our field of view, 60 degrees on each side
	//if (m_rotation < angle - 60 || m_rotation > angle + 60)
	//	return false;

	float x = targetPos.x - m_xPos;
	float y = targetPos.y - m_yPos;
	float length = sqrt((x*x) + (y*y));

	if (!length) return true;

	
	float unitX = x / length;
	float unitY = y / length;

	x = m_xPos;
	y = m_yPos;

	while(length > 0.1f)
	{
		if (CheckTile(XMFLOAT2(x, y))) return false;

		x += unitX * 0.3;
		y += unitY * 0.3;

		length = sqrt((targetPos.x - x) * (targetPos.x - x) + (targetPos.y - y) * (targetPos.y - y));

	}

	char s[128];
	sprintf_s(s, "In sight %d", num++);
	OutputDebugString(s);

	return true;
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

bool Monster::CheckTile(XMFLOAT2 pos)
{
	float currentDistance;
	float minDistance = pow(m_tileMap[0]->GetXPos() - pos.x, 2) + pow(m_tileMap[0]->GetYPos() - pos.y, 2);
	Tile* currentTile = m_tileMap[0];
	for (unsigned int i = 0; i < m_tileMap.size(); i++)
	{
		float startPosX = pos.x;
		float startPosY = pos.y;
		float nodePosX = m_tileMap[i]->GetXPos();
		float nodePosY = m_tileMap[i]->GetYPos();

		currentDistance = ((nodePosX - startPosX) * (nodePosX - startPosX)) + ((nodePosY - startPosY) * (nodePosY - startPosY));

		if (currentDistance < minDistance)
		{
			minDistance = currentDistance;
			currentTile = m_tileMap[i];
		}
	}

	if (!currentTile->GetIsWalkable()) return true;

	return false;
}
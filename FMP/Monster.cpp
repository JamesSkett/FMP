#include "Monster.h"
#include <time.h>


Monster::Monster(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height) :
	Entity(colour, x, y, z, scale, width, height)
{
	
}


Monster::~Monster()
{

}

void Monster::Update(XMFLOAT2 targetPos)
{

	if (pathfinder->GetIsPathFound())
	{
		if (waypointNum >= waypoints.size())
		{
			pathfinder->SetIsPathFound(false);
			waypointNum = 0;
		}
		else if (MoveTo(waypoints[waypointNum].x, waypoints[waypointNum].y))
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

bool Monster::MoveTo(float x, float y)
{

	m_dirX = x - m_xPos;
	m_dirY = y - m_yPos;

	float distance = sqrt(m_dirX * m_dirX + m_dirY * m_dirY);

	if (distance < 0.1) return true;

	m_dirX /= distance;
	m_dirY /= distance;

	m_xPos += m_dirX * m_speed;
	m_yPos += m_dirY * m_speed;

	return false;
}

void Monster::SetPathfinder(vector<Tile*> tilemap)
{
	pathfinder = new Pathfinding(tilemap);
	m_tileMap = tilemap;
}

bool Monster::LineOfSightCheck(XMFLOAT2 targetPos)
{
	XMFLOAT2 ray;

	ray.x = targetPos.x - m_xPos;
	ray.y = targetPos.y - m_yPos;



	return false;
}

void Monster::RandomWander()
{
	srand(time(NULL));
	int randTileNum = rand() % 901;

	if (pathfinder->GetIsPathFound())
	{
		if (waypointNum >= waypoints.size())
		{
			pathfinder->SetIsPathFound(false);
			waypointNum = 0;
		}
		else if (MoveTo(waypoints[waypointNum].x, waypoints[waypointNum].y))
		{
			waypointNum++;
		}
	}
	else
	{
		waypoints = pathfinder->FindPath(XMFLOAT2(m_xPos, m_yPos), XMFLOAT2(m_tileMap[randTileNum]->GetXPos(), m_tileMap[randTileNum]->GetYPos()));
	}
	
}

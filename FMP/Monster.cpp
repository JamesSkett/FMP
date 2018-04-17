#include "Monster.h"
#include "GameSystem.h"
#include <time.h>
#include <math.h>

#include "Math.h"


Monster::Monster(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height) :
	Entity(colour, x, y, z, scale, width, height)
{
	m_rotation = XMConvertToRadians(-90);
}


Monster::~Monster()
{
	

}
bool chaseStarted = false;
void Monster::Update(XMFLOAT2 targetPos, float deltaTime)
{
	m_viewCone->SetPos(m_xPos, m_yPos);
	m_viewCone->SetRotation(m_rotation);

	LineOfSightCheck(targetPos);


	if (m_playerInSight)
	{
		Chase(deltaTime);
		chaseStarted = true;
		waypoints.clear();
	}
	else if (chaseStarted)
	{
		m_isSearching = true;

		if (MoveTo(lastPlayerPos.x, lastPlayerPos.y, deltaTime))
		{
			chaseStarted = false;
			m_speed = 8.0f;
			m_timer = 3.0f;
		}
	}
	else if (!m_playerInSight && m_isSearching)
	{
		Search(targetPos, deltaTime);
	}
	else if (!m_playerInSight)
	{
		RandomWander(deltaTime);
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

bool Monster::GetPlayerInSight()
{
	return m_playerInSight;
}

bool Monster::GetIsSearching()
{
	return m_isSearching;
}

float Monster::GetRotation()
{
	return m_rotation;
}


bool Monster::MoveTo(float x, float y, float deltaTime)
{
	XMFLOAT2 oldPos = { m_xPos, m_yPos };
	
	m_rotation = -atan2f((x - m_xPos), (y - m_yPos));

	m_dirX = x - m_xPos;
	m_dirY = y - m_yPos;

	float distance = sqrt(m_dirX * m_dirX + m_dirY * m_dirY);

	if (distance < 0.1) return true;

	m_dirX /= distance;
	m_dirY /= distance;

	m_xPos += (m_dirX * m_speed) * deltaTime;
	if (CollisionCheck(m_tileMap))
	{
		m_xPos = oldPos.x;
	}

	m_yPos += (m_dirY * m_speed) * deltaTime;
	if (CollisionCheck(m_tileMap))
	{
		m_yPos = oldPos.y;
	}

	
	return false;
}

void Monster::SetPathfinder(vector<Tile*> tilemap)
{
	pathfinder = new Pathfinding(tilemap);
	m_tileMap = tilemap;
}

bool Monster::LineOfSightCheck(XMFLOAT2 targetPos)
{

	//get the direction the monster is looking
	float dirX = cosf(m_rotation + (XM_PI/2.f));
	float dirY = sinf(m_rotation + (XM_PI/2.f));

	//get the direction the player is from the monster
	XMVECTOR dir = XMVector2Normalize(XMVectorSet(targetPos.x - m_xPos, targetPos.y - m_yPos, 0, 0));

	//calculate the angle the angle based on the facing direction
	float angle = (float)acos(Math::Dot(XMFLOAT2(dirX, dirY), XMFLOAT2(XMVectorGetX(dir), XMVectorGetY(dir))));

	//if the angle is in the 50 degree cone
	if (XMConvertToDegrees(angle) >= 50)
	{
		m_playerInSight = false;
		return false;
	}

	float x = targetPos.x - m_xPos;
	float y = targetPos.y - m_yPos;
	float length = Math::Distance(XMFLOAT2(m_xPos, m_yPos), targetPos);

	if (!length)
	{
		m_playerInSight = true;
		lastPlayerPos = targetPos;
		return true;
	}
	if (length > 4)
	{
		m_playerInSight = false;
		return false;
	}

	float unitX = x / length;
	float unitY = y / length;

	x = m_xPos;
	y = m_yPos;

	while(length > 0.1f)
	{
		if (CheckTile(XMFLOAT2(x, y)))
		{
			m_playerInSight = false;
			return false;
		}

		x += unitX * 0.2f;
		y += unitY * 0.2f;

		length = sqrt((targetPos.x - x) * (targetPos.x - x) + (targetPos.y - y) * (targetPos.y - y));

	}

	char s[128];
	sprintf_s(s, "In sight %d", num++);
	OutputDebugString(s);

	m_playerInSight = true;
	lastPlayerPos = targetPos;

	return true;
}


void Monster::RandomWander(float deltaTime)
{
	int randTileNum = rand() % 901;

	m_viewCone->SetColour(Renderer::colour.Black);

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

void Monster::Chase(float deltaTime)
{
	if (m_playerInSight)
	{
		m_viewCone->SetColour(Renderer::colour.Green);
		m_speed = 12.0f;
		MoveTo(lastPlayerPos.x, lastPlayerPos.y, deltaTime);
	}
	
	
}

void Monster::Search(XMFLOAT2 playerPos, float deltaTime)
{
	int randTileNum = rand() % 901;

	m_viewCone->SetColour(Renderer::colour.Orange);

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
		float distance = sqrt((playerPos.x - m_tileMap[randTileNum]->GetXPos()) * (playerPos.x - m_tileMap[randTileNum]->GetXPos()) + (playerPos.y - m_tileMap[randTileNum]->GetYPos()) * (playerPos.y - m_tileMap[randTileNum]->GetYPos()));
		if (distance <= 0.8f)
		{
			waypoints = pathfinder->FindPath(XMFLOAT2(m_xPos, m_yPos), XMFLOAT2(m_tileMap[randTileNum]->GetXPos(), m_tileMap[randTileNum]->GetYPos()));
		}
	}

	if (m_timer <= 0)
	{
		m_isSearching = false;
	}

	m_timer -= deltaTime;

}

void Monster::Sneak(float playerPos, float deltaTime)
{
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
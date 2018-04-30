#include "Monster.h"
#include "GameSystem.h"
#include <time.h>
#include <math.h>

#include "Math.h"


int Monster::s_random_to_chase_or_sneak[2]   = { 15, 85 };
int Monster::s_random_to_chase_or_flee[2]    = { 15, 85 };
int Monster::s_chase_to_search_or_random[2]  = { 20, 80 };
int Monster::s_chase_to_flee_or_chase[2]     = { 80, 20 };
int Monster::s_search_to_sneak_or_chase[2]   = { 75, 25 };
int Monster::s_search_to_chase_or_flee[2]    = { 25, 75 };
int Monster::s_sneak_to_chase_or_flee[2]     = { 15, 85 };
int Monster::s_sneak_to_search_or_random[2]     = { 15, 85 };

Monster::Monster(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height) :
	Entity(colour, x, y, z, scale, width, height)
{

}


Monster::~Monster()
{
	for (unsigned int i = 0; i < m_waypoints.size(); i++)
	{
		delete m_waypoints[i];
		m_waypoints[i] = nullptr;
	}

	m_waypoints.clear();
}

void Monster::Update(Player* player, float deltaTime)
{
	if (Renderer::s_FogOfWar == 0)
	{
		m_viewCone->SetCanDraw(true);
	}
	else m_viewCone->SetCanDraw(false);
	
	m_viewCone->SetPos(m_xPos, m_yPos);
	m_viewCone->SetRotation(m_rotation);

	LineOfSightCheck(XMFLOAT2(player->GetXPos(), player->GetYPos()));

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

bool Monster::GetSoundHeard()
{
	return m_soundHeard;
}

XMFLOAT2 Monster::GetLastPlayerPos()
{
	return m_lastPlayerPos;
}


bool Monster::MoveTo(float x, float y, float deltaTime)
{
	XMFLOAT2 oldPos = { m_xPos, m_yPos };
	
	

	float distance = Math::Distance(XMFLOAT2(m_xPos, m_yPos), XMFLOAT2(x, y));

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

	m_rotation = -atan2f((x - m_xPos), (y - m_yPos));

	m_dirX = x - m_xPos;
	m_dirY = y - m_yPos;

	
	return false;
}

void Monster::SetPathfinder(vector<Tile*> tilemap)
{
	m_pathfinder = new Pathfinding(tilemap);
	m_tileMap = tilemap;
}

void Monster::SetSpeed(float value)
{
	m_speed = value;
}

void Monster::SetSoundHeard(bool value)
{
	m_soundHeard = value;
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

	//get the direction from monster to player
	float x = targetPos.x - m_xPos;
	float y = targetPos.y - m_yPos;
	//calculate the distance between the two
	float length = Math::Distance(XMFLOAT2(m_xPos, m_yPos), targetPos);
	if (!length)
	{
		m_playerInSight = true;
		m_lastPlayerPos = targetPos;
		return true;
	}
	//if the player is too far away we cant see them
	if (length > 4)
	{
		m_playerInSight = false;
		return false;
	}
	//get the unit direction
	float unitX = x / length;
	float unitY = y / length;
	//reset the x and y values
	x = m_xPos;
	y = m_yPos;
	//while the length is greater than 0.1
	while(length > 0.1f)
	{
		//is the current point's tile a wall
		if (CheckTile(XMFLOAT2(x, y)))
		{
			//if yes then player cant be seen
			m_playerInSight = false;
			return false;
		}
		//if not then move the point towards the player 
		x += unitX * 0.2f;
		y += unitY * 0.2f;
		//calculate the distnce
		length = Math::Distance(XMFLOAT2(x, y), targetPos);
	}
	//if the above doesnt hit a wall player is in sight
	m_playerInSight = true;
	m_lastPlayerPos = targetPos;

	return true;
}

void Monster::CheckHearing(XMFLOAT2 sourcePos, float radius)
{
	float distance = Math::Distance(XMFLOAT2(m_xPos, m_yPos), sourcePos);

	if (distance < radius)
	{
		m_soundLocation = sourcePos;
		m_soundHeard = true;
	}
}


void Monster::RandomWander(float deltaTime)
{
	int randTileNum = rand() % 901;

	m_speed = 6.0f;

	m_viewCone->SetColour(Renderer::colour.Black);

	if (m_pathfinder->GetIsPathFound())
	{
		if (m_waypointNum >= m_waypoints.size())
		{
			m_pathfinder->SetIsPathFound(false);
			m_waypointNum = 0;
		}
		else if (MoveTo(m_waypoints[m_waypointNum]->x, m_waypoints[m_waypointNum]->y, deltaTime))
		{
			m_waypointNum++;
		}
	}
	else
	{
		if (m_tileMap[randTileNum]->GetIsWalkable())
		{
			for (unsigned int i = 0; i < m_waypoints.size(); i++)
			{
				delete m_waypoints[i];
				m_waypoints[i] = nullptr;
			}

			m_waypoints.clear();
			m_waypointNum = 0;
			m_waypoints = m_pathfinder->FindPath(XMFLOAT2(m_xPos, m_yPos), XMFLOAT2(m_tileMap[randTileNum]->GetXPos(), m_tileMap[randTileNum]->GetYPos()));
		}
	}
	
}

void Monster::Chase(float deltaTime)
{
	if (m_playerInSight)
	{
		m_viewCone->SetColour(Renderer::colour.Green);
		m_speed = 12.0f;
		MoveTo(m_lastPlayerPos.x, m_lastPlayerPos.y, deltaTime);

		m_pathfinder->SetIsPathFound(false);
	}
	
	
	
}

void Monster::Search(XMFLOAT2 playerPos, float deltaTime)
{
	int randTileNum = rand() % 901;

	m_speed = 15.0f;

	m_viewCone->SetColour(Renderer::colour.Orange);

	if (m_pathfinder->GetIsPathFound())
	{
		if (m_waypointNum >= m_waypoints.size())
		{
			m_pathfinder->SetIsPathFound(false);
			m_waypointNum = 0;
			m_soundHeard = false;
		}
		else if (MoveTo(m_waypoints[m_waypointNum]->x, m_waypoints[m_waypointNum]->y, deltaTime))
		{
			m_waypointNum++;
		}
	}
	else
	{
		for (unsigned int i = 0; i < m_waypoints.size(); i++)
		{
			delete m_waypoints[i];
			m_waypoints[i] = nullptr;
		}
		m_waypoints.clear();
		m_waypointNum = 0;
		m_waypoints = m_pathfinder->FindPath(XMFLOAT2(m_xPos, m_yPos), playerPos);
		
	}

}

void Monster::Sneak(Player* player, float deltaTime)
{
	m_viewCone->SetColour(Renderer::colour.Blue);
	float distance = Math::Distance(XMFLOAT2(m_xPos, m_yPos), XMFLOAT2(player->GetXPos(), player->GetYPos()));

	if (distance < 3.5f && distance > 3 && m_playerInSight)
	{
		m_speed = 10;
		MoveTo(player->GetXPos(), player->GetYPos(), deltaTime);
	}
	else if (distance < 2 && m_playerInSight)
	{
		for (unsigned int i = 0; i < m_waypoints.size(); i++)
		{
			delete m_waypoints[i];
			m_waypoints[i] = nullptr;
		}
		m_waypoints.clear();
	}
	else if (m_pathfinder->GetIsPathFound())
	{
		if (m_waypointNum >= m_waypoints.size())
		{
			m_pathfinder->SetIsPathFound(false);
			m_waypointNum = 0;
		}
		else if (MoveTo(m_waypoints[m_waypointNum]->x, m_waypoints[m_waypointNum]->y, deltaTime))
		{
			m_waypointNum++;
		}
	}
	else
	{
		if (player->GetWalkedThroughDoor() && !m_playerInSight)
		{
			m_speed = 14;
			for (unsigned int i = 0; i < m_waypoints.size(); i++)
			{
				delete m_waypoints[i];
				m_waypoints[i] = nullptr;
			}
			m_waypoints.clear();
			m_waypointNum = 0;
			m_waypoints = m_pathfinder->FindPath(XMFLOAT2(m_xPos, m_yPos), player->GetDoorPos());

		}
		else if (!m_playerInSight || (m_playerInSight && distance > 2))
		{
			if (m_pathfinder->GetIsPathFound())
			{
				if (m_waypointNum >= m_waypoints.size())
				{
					m_pathfinder->SetIsPathFound(false);
					m_waypointNum = 0;
				}
				else if (MoveTo(m_waypoints[m_waypointNum]->x, m_waypoints[m_waypointNum]->y, deltaTime))
				{
					m_waypointNum++;
				}
			}
			else
			{
				m_speed = 14;
				for (unsigned int i = 0; i < m_waypoints.size(); i++)
				{
					delete m_waypoints[i];
					m_waypoints[i] = nullptr;
				}
				m_waypoints.clear();
				m_waypointNum = 0;
				m_waypoints = m_pathfinder->FindPath(XMFLOAT2(m_xPos, m_yPos), XMFLOAT2(player->GetXPos(), player->GetYPos()));
			}
		}
	}
	
}

void Monster::Flee(Player* player, float deltaTime)
{
	m_viewCone->SetColour(Renderer::colour.DimGray);
	int randTileNum = rand() % 901;

	m_speed = 30.0f;

	float currentDistance;

	
	if (m_pathfinder->GetIsPathFound())
	{
		if (m_waypointNum >= m_waypoints.size())
		{
			m_pathfinder->SetIsPathFound(false);
			m_waypointNum = 0;
		}
		else if (MoveTo(m_waypoints[m_waypointNum]->x, m_waypoints[m_waypointNum]->y, deltaTime))
		{
			m_waypointNum++;
		}
	}
	else
	{
		if (m_tileMap[randTileNum]->GetIsWalkable())
		{
			float startPosX = player->GetXPos();
			float startPosY = player->GetYPos();
			float tilePosX = m_tileMap[randTileNum]->GetXPos();
			float tilePosY = m_tileMap[randTileNum]->GetYPos();

			currentDistance = Math::Distance(XMFLOAT2(startPosX, startPosY), XMFLOAT2(tilePosX, tilePosY));

			//get the direction the player is looking
			float dirX = cosf(player->GetRotation() + (XM_PI / 2.f));
			float dirY = sinf(player->GetRotation() + (XM_PI / 2.f));

			//get the direction the tile is from the player
			XMVECTOR dir = XMVector2Normalize(XMVectorSet(tilePosX - startPosX, tilePosY - startPosY, 0, 0));

			//calculate the angle the angle based on the facing direction
			float angle = (float)acos(Math::Dot(XMFLOAT2(dirX, dirY), XMFLOAT2(XMVectorGetX(dir), XMVectorGetY(dir))));

			if (currentDistance > 6 && XMConvertToDegrees(angle) >= 60)
			{
				for (unsigned int i = 0; i < m_waypoints.size(); i++)
				{
					delete m_waypoints[i];
					m_waypoints[i] = nullptr;
				}
				m_waypoints.clear();
				m_waypointNum = 0;
				m_waypoints = m_pathfinder->FindPath(XMFLOAT2(m_xPos, m_yPos), XMFLOAT2(m_tileMap[randTileNum]->GetXPos(), m_tileMap[randTileNum]->GetYPos()));
			}
		}
	}
		
	
}

bool Monster::LookAround(float deltaTime)
{
	if (m_timer <= 0)
	{
		m_timer = 0.2f;
		return true;
	}
	m_timer -= deltaTime;

	m_rotation += 30 * deltaTime;

	return false;
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

		currentDistance = Math::Distance(XMFLOAT2(startPosX, startPosY), XMFLOAT2(nodePosX, nodePosY));

		if (currentDistance < minDistance)
		{
			minDistance = currentDistance;
			currentTile = m_tileMap[i];
		}
	}

	if (!currentTile->GetIsWalkable()) return true;

	return false;
}

XMFLOAT2 Monster::GetClosestDoorPos()
{
	float currentDistance;
	float minDistance = Math::Distance(XMFLOAT2(m_xPos, m_yPos), XMFLOAT2(m_tileMap[0]->GetXPos(), m_tileMap[0]->GetYPos()));
	Tile* currentTile = m_tileMap[0];
	for (unsigned int i = 0; i < m_tileMap.size(); i++)
	{
		if (m_tileMap[i]->GetIndex() == 3)
		{
			currentDistance = Math::Distance(XMFLOAT2(m_xPos, m_yPos), XMFLOAT2(m_tileMap[i]->GetXPos(), m_tileMap[i]->GetYPos()));

			if (currentDistance < minDistance)
			{
				minDistance = currentDistance;
				currentTile = m_tileMap[i];
			}
		}
	}

	return XMFLOAT2(currentTile->GetXPos(), currentTile->GetYPos());
}

void Monster::SetPathFound(bool value)
{
	m_pathfinder->SetIsPathFound(value);
}

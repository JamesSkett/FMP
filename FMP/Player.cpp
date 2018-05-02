#include "Player.h"
#include "Tile.h"
#include "GameSystem.h"
#include "Math.h"
#include <math.h>

float Player::s_rotation;
XMFLOAT2 Player::s_playerPos;

Player::Player(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height)
	: Entity(colour, x, y, z, scale, width, height)
{

}


Player::~Player()
{

}

void Player::Update(XMFLOAT2 enemyPos, vector <Tile*> tilemap, float deltaTime)
{
	if (Renderer::s_FogOfWar == 0)
	{
		m_viewCone->SetCanDraw(true);
	}
	else m_viewCone->SetCanDraw(false);

	m_viewCone->SetPos(m_xPos, m_yPos);
	m_viewCone->SetRotation(m_rotation);

	if (LineOfSightCheck(enemyPos, tilemap))
	{
		m_viewCone->SetColour(Renderer::colour.Green);
	}
	else
	{
		m_viewCone->SetColour(Renderer::colour.Black);
	}

	if (m_walkedThroughDoor)
	{
		if (m_timer <= 0)
		{
			m_walkedThroughDoor = false;
			m_timer = 0.02f;
		}

		m_timer -= deltaTime;
	}

	s_playerPos = { m_screenSpaceX, m_screenSpaceY };
}

void Player::SetXPos(float x)
{
	m_xPos = x;
}

void Player::SetYPos(float y)
{
	m_yPos = y;
}

void Player::SetZPos(float z)
{
	m_zPos = z;
}

void Player::SetScale(float scale)
{
	m_scale = scale;
}

float Player::GetXPos()
{
	return m_xPos;
}

float Player::GetYPos()
{
	return m_yPos;
}

float Player::GetZPos()
{
	return m_zPos;
}

float Player::GetScale()
{
	return m_scale;
}

float Player::GetRotation()
{
	return m_rotation;
}

float Player::GetDirectionX()
{
	return m_dirX;
}

float Player::GetDirectionY()
{
	return m_dirY;
}

bool Player::GetEnemyInSight()
{
	return m_enemyInSight;
}



void Player::UpdateXPos(vector <Tile*> tilemap, bool isRight, float deltaTime)
{
	float oldXPos = m_xPos;

	if (isRight)
	{
		m_xPos += (m_velocity * deltaTime);
		m_rotation = XMConvertToRadians(270);
	}
	else
	{
		m_xPos -= (m_velocity * deltaTime);
		m_rotation = XMConvertToRadians(90);
	}

	m_dirX = sinf(m_rotation);
	m_dirY = cosf(m_rotation);
	
	if (CollisionCheck(tilemap))
	{
		m_xPos = oldXPos;
	}
	
}

void Player::UpdateYPos(vector <Tile*> tilemap, bool isUp, float deltaTime)
{
	float oldYPos = m_yPos;

	if (isUp)
	{
		m_yPos += m_velocity * deltaTime;
		m_rotation = XMConvertToRadians(0);
	}
	else
	{
		 m_yPos -= m_velocity * deltaTime;
		 m_rotation = XMConvertToRadians(180);
	}

	m_dirX = sinf(m_rotation);
	m_dirY = cosf(m_rotation);

	if (CollisionCheck(tilemap))
	{
		m_yPos = oldYPos;
	}
}

void Player::UpdateZPos(float distance)
{
	m_zPos += distance;
}

void Player::UpdateScale(float scale)
{
	m_scale += scale;
}

void Player::LookAt(float targetX, float targetY)
{
	m_rotation = -atan2f((targetX - m_screenSpaceX), (targetY - m_screenSpaceY));

	m_dirX = sinf(m_rotation);
	m_dirY = cosf(m_rotation);
}

void Player::SprintOn()
{
	m_velocity = 20.0f;
}

void Player::SprintOff()
{
	m_velocity = 10.0f;
}


bool Player::LineOfSightCheck(XMFLOAT2 targetPos, vector <Tile*> tilemap)
{

	//get the direction the monster is looking
	float dirX = cosf(m_rotation + (XM_PI / 2.f));
	float dirY = sinf(m_rotation + (XM_PI / 2.f));

	//get the direction the player is from the monster
	XMVECTOR dir = XMVector2Normalize(XMVectorSet(targetPos.x - m_xPos, targetPos.y - m_yPos, 0, 0));

	//calculate the angle the angle based on the facing direction
	float angle = (float)acos(Math::Dot(XMFLOAT2(dirX, dirY), XMFLOAT2(XMVectorGetX(dir), XMVectorGetY(dir))));

	s_rotation = m_rotation;

	//if the angle is in the 50 degree cone
	if (XMConvertToDegrees(angle) >= 50)
	{
		m_enemyInSight = false;
		return false;
	}

	float x = targetPos.x - m_xPos;
	float y = targetPos.y - m_yPos;
	float length = Math::Distance(XMFLOAT2(m_xPos, m_yPos), targetPos);

	if (!length)
	{
		m_enemyInSight = true;
		return true;
	}
	if (length > c_viewDistance)
	{
		m_enemyInSight = false;
		return false;
	}

	float unitX = x / length;
	float unitY = y / length;

	x = m_xPos;
	y = m_yPos;

	while (length > 0.1f)
	{
		if (CheckTile(XMFLOAT2(x, y), tilemap))
		{
			m_enemyInSight = false;
			return false;
		}

		x += unitX * 0.2f;
		y += unitY * 0.2f;

		length = Math::Distance(XMFLOAT2(x, y), targetPos);
	}

	m_enemyInSight = true;

	return true;
}

bool Player::CheckTile(XMFLOAT2 pos, vector <Tile*> tilemap)
{
	float currentDistance;
	float minDistance = pow(tilemap[0]->GetXPos() - pos.x, 2) + pow(tilemap[0]->GetYPos() - pos.y, 2);
	Tile* currentTile = tilemap[0];
	for (unsigned int i = 0; i < tilemap.size(); i++)
	{
		float startPosX = pos.x;
		float startPosY = pos.y;
		float nodePosX = tilemap[i]->GetXPos();
		float nodePosY = tilemap[i]->GetYPos();

		currentDistance = Math::Distance(XMFLOAT2(startPosX, startPosY), XMFLOAT2(nodePosX, nodePosY));

		if (currentDistance < minDistance)
		{
			minDistance = currentDistance;
			currentTile = tilemap[i];
		}
	}

	if (!currentTile->GetIsWalkable()) return true;

	return false;
}
#include "Player.h"
#include "Tile.h"
#include "GameSystem.h"
#include <math.h>

Player::Player(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height)
	: Entity(colour, x, y, z, scale, width, height)
{

}


Player::~Player()
{

}

void Player::Update()
{
	m_viewCone->SetPos(m_xPos, m_yPos);
	m_viewCone->SetRotation(m_rotation);
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
	m_velocity = m_velocity * 2.0f;
}

void Player::SprintOff()
{
	m_velocity = 10.0f;
}




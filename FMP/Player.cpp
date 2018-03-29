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
	}
	else
	{
		m_xPos -= (m_velocity * deltaTime);
	}
	
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
	}
	else
	{
		 m_yPos -= m_velocity * deltaTime;
	}

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

bool Player::CollisionCheck(vector <Tile*> tilemap)
{
	for (unsigned int i = 0; i < tilemap.size(); i++)
	{
		if (tilemap[i]->GetIndex() == 2)
		{
			float box1x = m_xPos - (m_width/2);
			float box1y = m_yPos - (m_height/2);
			float box1w = m_width;
			float box1h = m_height;

			float box2x, box2y;
			float box2w, box2h;

			tilemap[i]->GetParameters(box2x, box2y, box2w, box2h);

			box2x = box2x - (box2w / 2);
			box2y = box2y - (box2h / 2);

			if ((box1x < box2x + box2w) && (box1x + box1w > box2x) && (box1y < box2y + box2h) && (box1h + box1y > box2y))
			{

				return true;
			}
		}
	}

	return false;
}

void Player::LookAt(float targetX, float targetY)
{
	m_rotation = -atan2f((targetX - m_screenSpaceX), (targetY - m_screenSpaceY));

	m_dirX = sinf(m_rotation);
	m_dirY = cosf(m_rotation);
}


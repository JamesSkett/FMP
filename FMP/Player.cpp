#include "Player.h"



Player::Player(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height)
	: Entity(colour, x, y, z, scale, width, height)
{

}


Player::~Player()
{

}

void Player::Update()
{
	colBox.x = m_xPos;
	colBox.y = m_yPos;
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

void Player::UpdateXPos(float distance)
{
	float oldXPos = m_xPos;
	m_xPos += distance;

	if (m_isColliding)
	{
		// if collision restore state
		m_xPos = oldXPos;
	}
}

void Player::UpdateYPos(float distance)
{
	float oldYPos = m_yPos;
	m_yPos += distance;

	if (m_isColliding)
	{
		// if collision restore state
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

bool Player::CollisionCheck(Entity* colObject)
{
	float box1x = colBox.x;
	float box1y = colBox.y;
	float box1w = colBox.w;
	float box1h = colBox.h;

	float box2x, box2y;
	float box2w, box2h;

	colObject->GetColBoxParameters(box2x, box2y, box2w, box2h);

	if (box1x < box2x + box2w && box1x + box1w > box2x && box1y < box2y + box2h && box1h + box1y > box2y)
	{
		m_isColliding = true;

		return true;
	}
	else
	{
		m_isColliding = false;
		return false;
	}

}

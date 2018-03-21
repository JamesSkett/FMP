#include "Monster.h"



Monster::Monster(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height) :
	Entity(colour, x, y, z, scale, width, height)
{

}


Monster::~Monster()
{

}

void Monster::Update(XMFLOAT2 targetPos)
{
	//MoveTo(targetPos.x, targetPos.y);
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
	if (m_xPos == x && m_yPos == y) return true;

	float moveAngle = atan2(y - m_yPos, x - m_xPos) * (180 / XM_PI);
	m_dirX = cosf(XMConvertToRadians(moveAngle));
	m_dirY = sinf(XMConvertToRadians(moveAngle));

	m_xPos += m_dirX * m_speed;
	m_yPos += m_dirY * m_speed;

	return false;
}

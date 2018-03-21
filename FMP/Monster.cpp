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

bool Monster::MoveTo(float x, float y, int &pointNum)
{
	if (m_xPos == x && m_yPos == y) return true;

	m_dirX = x - m_xPos;
	m_dirY = y - m_yPos;

	float distance = sqrt(m_dirX * m_dirX + m_dirY * m_dirY);

	if (distance < 0.1) pointNum++;

	m_dirX /= distance;
	m_dirY /= distance;

	m_xPos += m_dirX * m_speed;
	m_yPos += m_dirY * m_speed;

	return false;
}

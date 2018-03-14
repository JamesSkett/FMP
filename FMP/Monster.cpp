#include "Monster.h"



Monster::Monster(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height) :
	Entity(colour, x, y, z, scale, width, height)
{

}


Monster::~Monster()
{

}

float Monster::GetXPos()
{
	return m_xPos;
}

float Monster::GetYPos()
{
	return m_yPos;
}
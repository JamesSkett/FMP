#include "Projectile.h"
#include "Renderer.h"


Projectile::Projectile(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height):
	Entity(colour, x, y, x, scale, width, height)
{

}


Projectile::~Projectile()
{

}


void Projectile::Update(float dirX, float dirY)
{
	if (m_isFired)
	{
		m_xPos += dirX * m_velocity;
		m_yPos += dirY * m_velocity;
		m_isFired = true;
	}
}

bool Projectile::GetIsFired()
{
	return m_isFired;
}

void Projectile::SetXPos(float x)
{
	m_xPos = x;
}

void Projectile::SetYPos(float y)
{
	m_yPos = y;
}

void Projectile::SetIsFired(bool isFired)
{
	m_isFired = isFired;
}

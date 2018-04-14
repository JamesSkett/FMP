#include "Projectile.h"
#include "Renderer.h"


Projectile::Projectile(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height):
	Entity(colour, x, y, x, scale, width, height)
{

}


Projectile::~Projectile()
{

}


void Projectile::Update(float deltaTime)
{
	if (m_isFired)
	{
		m_xPos += (-m_dirX * m_velocity) * deltaTime;
		m_yPos += (m_dirY * m_velocity) * deltaTime;
		
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

void Projectile::SetDirection(float dx, float dy)
{
	m_dirX = dx;
	m_dirY = dy;
}

void Projectile::SetRotation(float angle)
{
	m_rotation = angle;
}



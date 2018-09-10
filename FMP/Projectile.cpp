#include "Projectile.h"
#include "Renderer.h"
#include "Tile.h"

Projectile::Projectile(float x, float y, float z, float scale, float width, float height, float depth):
	Entity(x, y, x, scale, width, height, depth)
{

}


Projectile::~Projectile()
{

}


void Projectile::Update(float deltaTime)
{
	if (m_isFired)
	{
		m_x += (-m_dx * m_velocity) * deltaTime;
		m_y += (m_dy * m_velocity) * deltaTime;
		
	}

}

bool Projectile::GetIsFired()
{
	return m_isFired;
}



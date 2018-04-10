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

bool Projectile::CollisionCheck(Entity* colObject)
{
	float box1x = m_xPos - (m_width / 2);
	float box1y = m_yPos - (m_height / 2);
	float box1w = m_width;
	float box1h = m_height;

	float box2x, box2y;
	float box2w, box2h;

	colObject->GetColBoxParameters(box2x, box2y, box2w, box2h);

	box2x = box2x - (box2w / 2);
	box2y = box2y - (box2h / 2);

	if ((box1x < box2x + box2w) && (box1x + box1w > box2x) && (box1y < box2y + box2h) && (box1h + box1y > box2y))
	{
		return true;
	}
	
	return false;
}

bool Projectile::CollisionCheck(vector <Tile*> tilemap)
{
	for (unsigned int i = 0; i < tilemap.size(); i++)
	{
		if (tilemap[i]->GetIndex() == 2)
		{
			float box1x = m_xPos - (m_width / 2);
			float box1y = m_yPos - (m_height / 2);
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
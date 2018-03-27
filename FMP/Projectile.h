#pragma once
#include "Entity.h"
#include "Tile.h"
#include <vector>

using namespace std;

class Projectile : public Entity
{
public:
	Projectile(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height);
	~Projectile();

	void Update();

	bool GetIsFired();

	void SetXPos(float x);
	void SetYPos(float y);
	void SetIsFired(bool isFired);
	void SetDirection(float dx, float dy);

	bool CollisionCheck(Entity* colObject);
	bool CollisionCheck(vector <Tile*> tilemap);

private:
	float m_velocity = 0.1f;

	float m_isFired = false;

	
};


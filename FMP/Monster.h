#pragma once
#include "Entity.h"
#include "Pathfinding.h"

class Monster : public Entity
{
public:
	Monster(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height);
	~Monster();

	void Update(XMFLOAT2 targetPos);

	float GetXPos();
	float GetYPos();

	bool MoveTo(float x, float y);

private:
	float m_speed = 0.1f;
};


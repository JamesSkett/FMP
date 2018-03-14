#pragma once
#include "Entity.h"
class Monster : public Entity
{
public:
	Monster(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height);
	~Monster();
	float GetXPos();
	float GetYPos();
};


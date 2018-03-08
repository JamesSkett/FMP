#pragma once
#include "Entity.h"

class Tile : public Entity
{
public:
	Tile(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height, bool isWall);
	~Tile();

private:
	bool m_isWall;
};


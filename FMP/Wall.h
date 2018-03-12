#pragma once
#include "Tile.h"

class Wall : public Tile
{
public:
	Wall(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height);
	~Wall();
};


#include "Floor.h"



Floor::Floor(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height, int index) :
	Tile(colour, x, y, z, scale, width, height, index)
{
	m_walkable = true;
}


Floor::~Floor()
{
}




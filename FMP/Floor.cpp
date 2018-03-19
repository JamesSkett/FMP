#include "Floor.h"



Floor::Floor(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height) :
	Tile(colour, x, y, z, scale, width, height)
{
	m_walkable = true;
	m_index = 1;
}


Floor::~Floor()
{
}




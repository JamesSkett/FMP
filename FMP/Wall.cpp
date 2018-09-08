#include "Wall.h"



Wall::Wall(float x, float y, float z, float scale, float width, float height, float depth, int index) :
	Tile(x, y, z, scale, width, height, depth, index)
{
	m_walkable = false;
}


Wall::~Wall()
{
}

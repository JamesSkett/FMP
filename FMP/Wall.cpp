#include "Wall.h"



Wall::Wall(char* texturefile, float x, float y, float z, float scale, float width, float height, int index) :
	Tile(x, y, z, scale, width, height, index)
{
	m_walkable = false;
}


Wall::~Wall()
{
}

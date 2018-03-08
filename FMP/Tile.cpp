#include "Tile.h"



Tile::Tile(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height, bool isWall) :
	Entity(colour, x, y, z, scale, width, height)
{
	m_isWall = isWall;
}


Tile::~Tile()
{

}

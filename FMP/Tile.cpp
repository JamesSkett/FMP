#include "Tile.h"



Tile::Tile(XMFLOAT4 colour, float x, float y, float z, float scale, bool isWall) :
	Entity(colour, x, y, z, scale)
{
	m_isWall = isWall;
}


Tile::~Tile()
{

}

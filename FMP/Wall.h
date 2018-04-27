/*
	Class:		Wall
	Parent:		Tile
	Project:	Adaptive Monster AI for a Horror Game (Final Major Project)
	Author:		James Skett
	Purpose:	Used to differenciate between the floor by using the index value
*/

#pragma once
#include "Tile.h"

class Wall : public Tile
{
public:
	Wall(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height, int index);
	~Wall();
};


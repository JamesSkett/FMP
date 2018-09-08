/*
	Class:		Floor
	Parent:		Tile
	Project:	Adaptive Monster AI for a Horror Game (Final Major Project)
	Author:		James Skett
	Purpose:	Used to differenciate between the walls by using the index value
*/

#pragma once
#include "Tile.h"
class Floor :
	public Tile
{
public:
	Floor(float x, float y, float z, float scale, float width, float height, float depth, int index);
	~Floor();

	

private:

};


/*
	Class:		Projectile
	Parent:		Entity
	Project:	Adaptive Monster AI for a Horror Game (Final Major Project)
	Author:		James Skett
*/

#pragma once
#include "Entity.h"
#include "Tile.h"
#include <vector>

using namespace std;

class Projectile : public Entity
{
public:
	Projectile(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height);
	~Projectile();

	//Moves the projectile if fired
	void Update(float deltaTime);

	void Draw(XMMATRIX view, XMMATRIX projection);

	//Get the value of m_isFired
	bool GetIsFired();

	//Set the x position value
	void SetXPos(float x);
	//Set the y position value
	void SetYPos(float y);
	//Set m_isFired to true or false
	void SetIsFired(bool isFired);
	//Set the directional values for the projectile
	void SetDirection(float dx, float dy);
	//Set the roatation value for the projectile
	void SetRotation(float angle);


private:
	float m_velocity = 50.0f;

	float m_isFired = false;

	
};


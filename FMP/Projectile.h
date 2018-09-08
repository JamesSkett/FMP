/*
	Class:		Projectile
	Parent:		Entity
	Project:	Adaptive Monster AI for a Horror Game (Final Major Project)
	Author:		James Skett
*/

#pragma once
#include "Entity.h"
#include <vector>

using namespace std;

class Projectile : public Entity
{
public:
	Projectile(float x, float y, float z, float scale, float width, float height);
	~Projectile();

	//Moves the projectile if fired
	void Update(float deltaTime);

	void Draw(XMMATRIX view, XMMATRIX projection);

	//Get the value of m_isFired
	bool GetIsFired();


private:
	float m_velocity = 50.0f;

	float m_isFired = false;

	
};


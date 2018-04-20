#pragma once

#include "Entity.h"
#include "Tile.h"
#include "Projectile.h"
#include <vector>

using namespace std;

class Player : public Entity
{
public:
	Player(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height);
	~Player();

	void Update(XMFLOAT2 enemyPos, vector <Tile*> tilemap, float deltaTime);

	//Set a new x position value
	void SetXPos(float x);
	//set a new y position value
	void SetYPos(float y);
	//Set a new z position value
	void SetZPos(float z);
	//Set a new scale value
	void SetScale(float scale);


	//Get the current x position value
	float GetXPos();
	//Get the current y position value
	float GetYPos();
	//Get the current z position value
	float GetZPos();
	//Get the current scalar value
	float GetScale();
	//Get the current rotation value
	float GetRotation();
	//Get the current x direction value
	float GetDirectionX();
	//Get the current y direction value
	float GetDirectionY();
	//Get the value of m_enemyInSight
	bool GetEnemyInSight();

	//Update the x position value
	void UpdateXPos(vector <Tile*> colObject, bool isRight, float deltaTime);
	//Update the y position value
	void UpdateYPos(vector <Tile*> colObject, bool isUp, float deltaTime);
	//Update the z position value
	void UpdateZPos(float distance);
	//Update the scalar value
	void UpdateScale(float scale);

	//Rotate the player to look at a specified point targetX & targetY
	void LookAt(float targetX, float targetY);

	//Increases the player speed
	void SprintOn();
	//Resets the player speed to normal
	void SprintOff();

	//Checks to see if the player is in sight
	bool LineOfSightCheck(XMFLOAT2 targetPos, vector <Tile*> tilemap);

	//Checks if the tile at 'pos' is walkable
	bool CheckTile(XMFLOAT2 pos, vector <Tile*> tilemap);

private:
	//Player move speed
	float m_velocity = 10.0f;

	bool m_enemyInSight = false;

	float m_timer = 0.3f;
};


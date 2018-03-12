#pragma once

#include "Entity.h"
#include "Tile.h"
#include <vector>

using namespace std;

class Player : public Entity
{
public:
	Player(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height);
	~Player();

	void Update();
	void ResetPlayerPos();

	void SetXPos(float x);
	void SetYPos(float y);
	void SetZPos(float z);
	void SetScale(float scale);
	void SetPlayerVelocity(float velocity);
	void SetTilemap(vector <string> tilemap);

	float GetXPos();
	float GetYPos();
	float GetZPos();
	float GetScale();

	void UpdateXPos(vector <Tile*> colObject, bool isRight);
	void UpdateYPos(vector <Tile*> colObject, bool isUp);
	void UpdateZPos(float distance);
	void UpdateScale(float scale);

	bool CollisionCheck();

	float angle;

private:
	bool m_isColliding;
	float m_previousXPos, m_previousYPos;

	float m_velocity = 0.05f;

	vector <string> m_tilemap;
};


#pragma once

#include "Tile.h"
#include <vector>

using namespace std;

class Player : public Entity
{
public:
	Player(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height);
	~Player();

	void Update();

	void SetXPos(float x);
	void SetYPos(float y);
	void SetZPos(float z);
	void SetScale(float scale);

	float GetXPos();
	float GetYPos();
	float GetZPos();
	float GetScale();

	void UpdateXPos(float distance, vector <Tile*> colObject);
	void UpdateYPos(float distance, vector <Tile*> colObject);
	void UpdateZPos(float distance);
	void UpdateScale(float scale);

	bool CollisionCheck(vector <Tile*> colObject);

private:
	bool m_isColliding;
};


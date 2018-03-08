#pragma once
#include "Entity.h"
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

	void UpdateXPos(float distance);
	void UpdateYPos(float distance);
	void UpdateZPos(float distance);
	void UpdateScale(float scale);

	bool CollisionCheck(Entity* colObject);

private:
	bool m_isColliding;
};


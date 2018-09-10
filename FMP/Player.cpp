#include "Player.h"
#include "Tile.h"
#include "GameSystem.h"
#include "Math.h"
#include "Renderer.h"
#include <math.h>

float Player::s_rotation;
XMFLOAT2 Player::s_playerPos;

Player::Player(float x, float y, float z, float scale, float width, float height)
	: Entity(x, y, z, scale, width, height)
{

}


Player::~Player()
{

}

void Player::Update(XMFLOAT2 enemyPos, vector <Tile*> tilemap, float deltaTime)
{
	

	if (m_walkedThroughDoor)
	{
		if (m_timer <= 0)
		{
			m_walkedThroughDoor = false;
			m_timer = 0.02f;
		}

		m_timer -= deltaTime;
	}

	s_playerPos = { m_screenSpaceX, m_screenSpaceY };
}

bool Player::GetEnemyInSight()
{
	return m_enemyInSight;
}

void Player::SprintOn()
{
	m_velocity = 20.0f;
}

void Player::SprintOff()
{
	m_velocity = 10.0f;
}


bool Player::LineOfSightCheck(XMFLOAT2 targetPos, vector <Tile*> tilemap)
{

	//get the direction the monster is looking
	float dirX = cosf(m_xangle + (XM_PI / 2.f));
	float dirY = sinf(m_yangle + (XM_PI / 2.f));

	//get the direction the player is from the monster
	XMVECTOR dir = XMVector2Normalize(XMVectorSet(targetPos.x - m_x, targetPos.y - m_y, 0, 0));

	//calculate the angle the angle based on the facing direction
	float angle = (float)acos(Math::Dot(XMFLOAT2(dirX, dirY), XMFLOAT2(XMVectorGetX(dir), XMVectorGetY(dir))));

	//s_rotation = m_rotation;

	//if the angle is in the 50 degree cone
	if (XMConvertToDegrees(angle) >= 50)
	{
		m_enemyInSight = false;
		return false;
	}

	float x = targetPos.x - m_x;
	float y = targetPos.y - m_y;
	float length = Math::Distance(XMFLOAT2(m_x, m_y), targetPos);

	if (!length)
	{
		m_enemyInSight = true;
		return true;
	}
	if (length > c_viewDistance)
	{
		m_enemyInSight = false;
		return false;
	}

	float unitX = x / length;
	float unitY = y / length;

	x = m_x;
	y = m_y;

	while (length > 0.1f)
	{
		if (CheckTile(XMFLOAT2(x, y), tilemap))
		{
			m_enemyInSight = false;
			return false;
		}

		x += unitX * 0.2f;
		y += unitY * 0.2f;

		length = Math::Distance(XMFLOAT2(x, y), targetPos);
	}

	m_enemyInSight = true;

	return true;
}

bool Player::CheckTile(XMFLOAT2 pos, vector <Tile*> tilemap)
{
	float currentDistance;
	float minDistance = pow(tilemap[0]->GetXPos() - pos.x, 2) + pow(tilemap[0]->GetYPos() - pos.y, 2);
	Tile* currentTile = tilemap[0];
	for (unsigned int i = 0; i < tilemap.size(); i++)
	{
		float startPosX = pos.x;
		float startPosY = pos.y;
		float nodePosX = tilemap[i]->GetXPos();
		float nodePosY = tilemap[i]->GetYPos();

		currentDistance = Math::Distance(XMFLOAT2(startPosX, startPosY), XMFLOAT2(nodePosX, nodePosY));

		if (currentDistance < minDistance)
		{
			minDistance = currentDistance;
			currentTile = tilemap[i];
		}
	}

	if (!currentTile->GetIsWalkable()) return true;

	return false;
}
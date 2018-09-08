/*
	Class:		Monster
	Parent:		Entity
	Project:	Adaptive Monster AI for a Horror Game (Final Major Project)
	Author:		James Skett
	Purpose:	This is where the main AI behaviours are and where the pathfinding object is created
*/

#pragma once


#include <vector>

using namespace std;

#include "Entity.h"

class Tile;
class Pathfinding;
class Player;
class Asset;

class Monster : public Entity
{
public:
	Monster(float x, float y, float z, float scale, float width, float height);
	~Monster();

	//Updates the monster 
	void Update(Player* player, float deltaTime);

	//Get the value of m_playerInSight
	bool GetPlayerInSight() const;
	//Get the value of m_isSearching
	bool GetIsSearching() const;
	//Gets the value of m_soundHeard
	bool GetSoundHeard() const;
	//Gets the last location the player was seen.
	XMFLOAT2 GetLastPlayerPos() const;

	//Moves to a specified point
	bool MoveTo(float x, float y, float deltaTime);

	//Creates the pathfinder object and gives it the tilemap
	void SetPathfinder(vector <Tile*> tilemap);
	//Set the value of m_speed
	void SetSpeed(float value);
	//Set the value of m_soundheard
	void SetSoundHeard(bool value);

	//Check to see if the player is in sight
	bool LineOfSightCheck(XMFLOAT2 targetPos);
	//Checks if the monster has heard a sound
	void CheckHearing(XMFLOAT2 sourcePos, float radius);

	//Behaviours
	void RandomWander(float deltaTime);
	void Chase(float deltaTime);
	void Search(XMFLOAT2 playerPos, float deltaTime);  
	void Sneak(Player* player, float deltaTime);
	void Flee(Player* player, float deltaTime);
	bool LookAround(float deltaTime);

	bool CheckTile(XMFLOAT2 pos);

	XMFLOAT2 GetClosestDoorPos();

	void SetPathFound(bool value);

	//state transition probabilites
	static int s_random_to_chase_or_sneak[2];
	static int s_random_to_chase_or_flee[2];
	static int s_chase_to_search_or_random[2];
	static int s_chase_to_flee_or_chase[2];
	static int s_search_to_sneak_or_chase[2];
	static int s_search_to_chase_or_flee[2];
	static int s_sneak_to_chase_or_flee[2];
	static int s_sneak_to_search_or_random[2];

private:
	float m_speed = 6.0f;
	float m_timer = 0.2f;

	Pathfinding* m_pathfinder;


	vector <XMFLOAT2*> m_waypoints;

	vector <Tile*> m_tileMap;

	unsigned int m_waypointNum = 0;

	int num = 0;

	bool m_playerInSight = false;
	bool m_isSearching = false;
	bool m_isSneaking = false;
	bool m_isFleeing = false;
	bool m_soundHeard = false;

	XMFLOAT2 m_lastPlayerPos;
	XMFLOAT2 m_soundLocation;
};


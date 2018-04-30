/*
	Class:		Floor
	Project:	Adaptive Monster AI for a Horror Game (Final Major Project)
	Author:		James Skett
	Purpose:	Controls the AI state transitions
*/

#pragma once
#include "Monster.h"
#include "Player.h"

enum State
{
	RANDOM_WANDER	= 0,
	CHASE			= 1,
	SEARCH			= 2,
	SNEAK			= 3,
	FLEE			= 4,
	ATTACK			= 5
};

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void RunStateMachine	(Player* player, Monster* monster, float deltaTime);
	void SetCurrentState(State state);

	string GetCurrentState();

	static bool s_chasing;
	

private:
	State IsRandomWander	(Player* player, Monster* monster);
	State IsChasing			(Player* player, Monster* monster, float deltaTime);
	State IsSearching		(Player* player, Monster* monster, float deltaTime);
	State IsSneaking		(Player* player, Monster* monster, float deltaTime);
	State IsFleeing			(Player* player, Monster* monster, float deltaTime);
	State IsAttacking       (Player* player, Monster* monster);

	State m_currentState = RANDOM_WANDER;
	string m_currentStateText;

	float m_searchTimer  = 1.0f;
	float m_sneakTimer   = 1.0f;
	float m_attackTimer  = 1.0f;
	float m_fleeTimer    = 1.0f;

	const int weightChange = 10;
	const int WEIGHTING_MIN = 1;
	const int WEIGHTING_MAX = 100;


};


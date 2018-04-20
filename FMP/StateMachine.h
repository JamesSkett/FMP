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

private:
	State IsRandomWander	(Player* player, Monster* monster);
	State IsChasing			(Player* player, Monster* monster);
	State IsSearching		(Player* player, Monster* monster);
	State IsSneaking		(Player* player, Monster* monster);
	State IsFleeing			(Player* player, Monster* monster, float deltaTime);
	State IsAttacking       (Player* player, Monster* monster);

	State m_currentState = RANDOM_WANDER;

	float m_searchTimer  = 3.0f;
	float m_sneakTimer   = 3.0f;
	float m_fleeTimer    = 2.0f;

	int WEIGHTING_MIN = 1;
	int WEIGHTING_MAX = 100;

	//random to chase
	//random to sneak
	//sneak to chase
	//chase to flee
	//chase to attack
};


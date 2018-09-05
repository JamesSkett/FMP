/*
	Class:		Floor
	Project:	Adaptive Monster AI for a Horror Game (Final Major Project)
	Author:		James Skett
	Purpose:	Controls the AI state transitions
*/

#pragma once

#include <string>

using namespace std;

class Player;
class Monster;

//Outlines what the different states are
enum State
{
	RANDOM_WANDER	= 0,
	CHASE			= 1,
	SEARCH			= 2,
	SNEAK			= 3,
	FLEE			= 4,
};

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	//Runs the state machine
	void RunStateMachine(Player* player, Monster* monster, float deltaTime);
	//Sets the current state of the monster
	void SetCurrentState(State state);

	//Gets the current state of the monster as a string
	string GetCurrentState() const;

	static bool s_chasing;
	

private:
	//condition check functions
	State IsRandomWander	(Player* player, Monster* monster);
	State IsChasing			(Player* player, Monster* monster, float deltaTime);
	State IsSearching		(Player* player, Monster* monster, float deltaTime);
	State IsSneaking		(Player* player, Monster* monster, float deltaTime);
	State IsFleeing			(Player* player, Monster* monster, float deltaTime);

	State m_currentState = RANDOM_WANDER;
	string m_currentStateText;

	float m_searchTimer  = 2.0f;
	float m_sneakTimer   = 1.0f;
	float m_attackTimer  = 1.0f;
	float m_fleeTimer    = 0.8f;

	const int weightChange = 10;
	const int WEIGHTING_MIN = 1;
	const int WEIGHTING_MAX = 100;


};


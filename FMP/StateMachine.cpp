#include "StateMachine.h"
#include "Math.h"


StateMachine::StateMachine()
{
}

StateMachine::~StateMachine()
{
}

void StateMachine::RunStateMachine(Player* player, Monster* monster, float deltaTime)
{
	switch (m_currentState)
	{
	case RANDOM_WANDER:
		monster->RandomWander(deltaTime);
		m_currentState = IsRandomWander(player, monster);
		break;
	case CHASE:
		monster->Chase(deltaTime);
		m_currentState = IsChasing(player, monster);
		break;
	case SEARCH:
		monster->Search(XMFLOAT2(player->GetXPos(), player->GetYPos()), deltaTime);
		m_currentState = IsSearching(player, monster);
		break;
	case SNEAK:
		monster->Sneak(player, deltaTime);
		m_currentState = IsSneaking(player, monster);
		break;
	case FLEE:
		monster->Flee(player, deltaTime);
		m_currentState = IsFleeing(player, monster);
		break;
	}
}

State StateMachine::IsRandomWander(Player* player, Monster* monster)
{
	if (monster->GetPlayerInSight() && !player->GetEnemyInSight())
	{
		return SNEAK;
	}
	else if (monster->GetPlayerInSight())
	{
		return CHASE;
	}

	return RANDOM_WANDER;
}

State StateMachine::IsChasing(Player* player, Monster* monster)
{
	if (!monster->GetPlayerInSight())
	{
		return SEARCH;
	}
	else if (monster->GetPlayerInSight() && player->GetEnemyInSight())
	{
		return FLEE;
	}

	return CHASE;
}

State StateMachine::IsSearching(Player* player, Monster* monster)
{
	if (monster->GetPlayerInSight() && !player->GetEnemyInSight())
	{
		return SNEAK;
	}
	else if (monster->GetPlayerInSight() && player->GetEnemyInSight())
	{
		return FLEE;
	}
	else if (monster->GetPlayerInSight())
	{
		return CHASE;
	}

	return SEARCH;
}

State StateMachine::IsSneaking(Player* player, Monster* monster)
{
	if (monster->GetPlayerInSight() && player->GetEnemyInSight())
	{
		return FLEE;
	}

	return SNEAK;
}

State StateMachine::IsFleeing(Player* player, Monster* monster)
{
	if (!player->GetEnemyInSight() || Math::Distance(XMFLOAT2(monster->GetXPos(), monster->GetYPos()), XMFLOAT2(player->GetXPos(), player->GetYPos())) > 8)
	{
		if (m_fleeTimer <= 0)
		{
			m_fleeTimer = 2.0f;
			return RANDOM_WANDER;
		}
	}
	else m_fleeTimer = 2.0f;

	return FLEE;
}

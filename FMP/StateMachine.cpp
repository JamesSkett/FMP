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
		m_currentStateText = "Random Wander";
		monster->RandomWander(deltaTime);
		m_currentState = IsRandomWander(player, monster);
		break;
	case CHASE:
		m_currentStateText = "Chase";
		monster->Chase(deltaTime);
		m_currentState = IsChasing(player, monster);
		break;
	case SEARCH:
		m_currentStateText = "Search";
		monster->Search(XMFLOAT2(player->GetXPos(), player->GetYPos()), deltaTime);
		m_currentState = IsSearching(player, monster, deltaTime);
		break;
	case SNEAK:
		m_currentStateText = "Sneak";
		monster->Sneak(player, deltaTime);
		m_currentState = IsSneaking(player, monster);
		break;
	case FLEE:
		m_currentStateText = "Flee";
		monster->Flee(player, deltaTime);
		m_currentState = IsFleeing(player, monster, deltaTime);
		break;
	case ATTACK:

		break;
	}
}

string StateMachine::GetCurrentState()
{
	return m_currentStateText;
}

State StateMachine::IsRandomWander(Player* player, Monster* monster)
{
	if (monster->GetPlayerInSight() && !player->GetEnemyInSight())
	{
		float chaseProbability = ((float)Monster::s_random_to_chase_or_sneak[0] / (float)WEIGHTING_MAX) * 100;
		float sneakProbability = ((float)Monster::s_random_to_chase_or_sneak[1] / (float)WEIGHTING_MAX) * 100;

		int weight = rand() % WEIGHTING_MAX + WEIGHTING_MIN;

		if (weight <= WEIGHTING_MAX && weight > WEIGHTING_MAX - sneakProbability)
		{
			monster->SetPathFound(false);
			return SNEAK;
		}
		else if (weight <= (WEIGHTING_MIN + chaseProbability - 1) && weight >= WEIGHTING_MIN)
		{
			monster->SetPathFound(false);
			return CHASE;
		}
	}
	else if (monster->GetPlayerInSight() && player->GetEnemyInSight())
	{
		float chaseProbability = ((float)Monster::s_random_to_chase_or_flee[0] / (float)WEIGHTING_MAX) * 100;
		float fleeProbability = ((float)Monster::s_random_to_chase_or_flee[1] / (float)WEIGHTING_MAX) * 100;

		int weight = rand() % WEIGHTING_MAX + WEIGHTING_MIN;

		if (weight <= WEIGHTING_MIN + (chaseProbability - 1) && weight >= WEIGHTING_MIN)
		{
			monster->SetPathFound(false);
			return CHASE;
		}
		else if (weight <= WEIGHTING_MAX && weight > WEIGHTING_MAX - fleeProbability)
		{
			monster->SetPathFound(false);
			return FLEE;
		}
	}
	else if (monster->GetSoundHeard())
	{
		monster->SetPathFound(false);
		monster->SetSoundHeard(false);
		return SEARCH;
	}

	return RANDOM_WANDER;
}

State StateMachine::IsChasing(Player* player, Monster* monster)
{
	if (!monster->GetPlayerInSight())
	{
		float searchProbability = ((float)Monster::s_chase_to_search_or_random[0] / (float)WEIGHTING_MAX) * 100;
		float randomWandProbability = ((float)Monster::s_chase_to_search_or_random[1] / (float)WEIGHTING_MAX) * 100;

		int weight = rand() % WEIGHTING_MAX + WEIGHTING_MIN;

		if (weight <= WEIGHTING_MIN + (searchProbability - 1) && weight >= WEIGHTING_MIN)
		{
			monster->SetPathFound(false);
			return SEARCH;
		}
		else if (weight <= WEIGHTING_MAX && weight > WEIGHTING_MAX - randomWandProbability)
		{
			monster->SetPathFound(false);
			return RANDOM_WANDER;
		}
	}
	else if (monster->GetPlayerInSight() && player->GetEnemyInSight())
	{
		monster->SetPathFound(false);
		return FLEE;

		//or chase
	}

	return CHASE;
}

State StateMachine::IsSearching(Player* player, Monster* monster, float deltaTime)
{
	if (monster->GetPlayerInSight() && !player->GetEnemyInSight())
	{
		int weight = rand() % WEIGHTING_MAX + WEIGHTING_MIN;

		if (weight <= WEIGHTING_MAX && weight > 15)
		{
			monster->SetPathFound(false);
			return SNEAK;
		}
		else if (weight <= 15 && weight >= WEIGHTING_MIN)
		{
			monster->SetPathFound(false);
			return CHASE;
		}
	}
	else if (monster->GetPlayerInSight() && player->GetEnemyInSight())
	{
		int weight = rand() % 100 + 1;

		if (weight <= 30 && weight >= WEIGHTING_MIN)
		{
			monster->SetPathFound(false);
			return CHASE;
		}
		else if (weight <= WEIGHTING_MAX && weight > 30)
		{
			monster->SetPathFound(false);
			return FLEE;
		}
	}
	else if (!monster->GetPlayerInSight())
	{
		if (m_searchTimer <= 0)
		{
			m_searchTimer = 1.0f;
			monster->SetPathFound(false);
			return RANDOM_WANDER;
		}
		m_searchTimer -= deltaTime;
	}


	return SEARCH;
}

State StateMachine::IsSneaking(Player* player, Monster* monster)
{
	if (monster->GetPlayerInSight() && player->GetEnemyInSight())
	{
		monster->SetPathFound(false);
		return FLEE;

		//or chase
	}

	return SNEAK;
}

State StateMachine::IsFleeing(Player* player, Monster* monster, float deltaTime)
{
	if (!player->GetEnemyInSight() || Math::Distance(XMFLOAT2(monster->GetXPos(), monster->GetYPos()), XMFLOAT2(player->GetXPos(), player->GetYPos())) > 8)
	{
		if (m_fleeTimer <= 0)
		{
			m_fleeTimer = 1.0f;
			monster->SetPathFound(false);
			return RANDOM_WANDER;
		}
		m_fleeTimer -= deltaTime;
	}
	else m_fleeTimer = 1.0f;

	return FLEE;
}

State StateMachine::IsAttacking(Player * player, Monster * monster)
{
	return ATTACK;
}

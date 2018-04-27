#include "StateMachine.h"
#include "Math.h"

bool StateMachine::s_chasing = false;

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
		m_currentState = IsChasing(player, monster, deltaTime);
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

void StateMachine::SetCurrentState(State state)
{
	m_currentState = state;
}

string StateMachine::GetCurrentState()
{
	return m_currentStateText;
}

State StateMachine::IsRandomWander(Player* player, Monster* monster)
{
	if (monster->GetPlayerInSight() && !player->GetEnemyInSight())
	{
		int chaseProbability = Monster::s_random_to_chase_or_sneak[0];
		int sneakProbability = Monster::s_random_to_chase_or_sneak[1];

		int weight = rand() % WEIGHTING_MAX + WEIGHTING_MIN;

		if (weight <= WEIGHTING_MAX && weight > WEIGHTING_MAX - sneakProbability)
		{
			monster->SetPathFound(false);
			if (Monster::s_random_to_chase_or_sneak[1] >= WEIGHTING_MIN)
			{
				Monster::s_random_to_chase_or_sneak[1]--;
			}

			if (Monster::s_random_to_chase_or_sneak[0] <= WEIGHTING_MAX)
			{
				Monster::s_random_to_chase_or_sneak[0]++;
			}

			return SNEAK;
		}
		else if (weight <= (WEIGHTING_MIN + chaseProbability - 1) && weight >= WEIGHTING_MIN)
		{
			monster->SetPathFound(false);
			s_chasing = true;

			if (Monster::s_random_to_chase_or_sneak[1] >= WEIGHTING_MIN)
			{
				Monster::s_random_to_chase_or_sneak[1]--;
			}

			if (Monster::s_random_to_chase_or_sneak[0] <= WEIGHTING_MAX)
			{
				Monster::s_random_to_chase_or_sneak[0]++;
			}

			return CHASE;
		}
	}
	else if (monster->GetPlayerInSight() && player->GetEnemyInSight())
	{
		int chaseProbability = Monster::s_random_to_chase_or_flee[0];
		int fleeProbability = Monster::s_random_to_chase_or_flee[1];

		int weight = rand() % WEIGHTING_MAX + WEIGHTING_MIN;

		if (weight <= WEIGHTING_MIN + (chaseProbability - 1) && weight >= WEIGHTING_MIN)
		{
			monster->SetPathFound(false);
			s_chasing = true;

			if (Monster::s_random_to_chase_or_flee[1] >= WEIGHTING_MIN)
			{
				Monster::s_random_to_chase_or_flee[1]--;
			}

			if (Monster::s_random_to_chase_or_flee[0] <= WEIGHTING_MAX)
			{
				Monster::s_random_to_chase_or_flee[0]++;
			}

			return CHASE;
		}
		else if (weight <= WEIGHTING_MAX && weight > WEIGHTING_MAX - fleeProbability)
		{
			monster->SetPathFound(false);

			if (Monster::s_random_to_chase_or_flee[1] >= WEIGHTING_MIN)
			{
				Monster::s_random_to_chase_or_flee[1]--;
			}

			if (Monster::s_random_to_chase_or_flee[0] <= WEIGHTING_MAX)
			{
				Monster::s_random_to_chase_or_flee[0]++;
			}

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

State StateMachine::IsChasing(Player* player, Monster* monster, float deltaTime)
{
	if (!monster->GetPlayerInSight())
	{
		if (!monster->MoveTo(monster->GetLastPlayerPos().x, monster->GetLastPlayerPos().y, deltaTime))
		{
			return CHASE;
		}

		if (monster->LookAround(deltaTime))
		{
			s_chasing = false;
		}
		else return CHASE;

		int searchProbability = Monster::s_chase_to_search_or_random[0];
		int randomWandProbability = Monster::s_chase_to_search_or_random[1];

		int weight = rand() % WEIGHTING_MAX + WEIGHTING_MIN;

		if (weight <= WEIGHTING_MIN + (searchProbability - 1) && weight >= WEIGHTING_MIN)
		{
			monster->SetPathFound(false);

			if (Monster::s_chase_to_search_or_random[1] >= WEIGHTING_MIN)
			{
				Monster::s_chase_to_search_or_random[1]--;
			}

			if (Monster::s_chase_to_search_or_random[0] <= WEIGHTING_MAX)
			{
				Monster::s_chase_to_search_or_random[0]++;
			}

			return SEARCH;
		}
		else if (weight <= WEIGHTING_MAX && weight > WEIGHTING_MAX - randomWandProbability)
		{
			monster->SetPathFound(false);

			if (Monster::s_chase_to_search_or_random[1] >= WEIGHTING_MIN)
			{
				Monster::s_chase_to_search_or_random[1]--;
			}

			if (Monster::s_chase_to_search_or_random[0] <= WEIGHTING_MAX)
			{
				Monster::s_chase_to_search_or_random[0]++;
			}

			return RANDOM_WANDER;
		}
	}
	else if (monster->GetPlayerInSight() && player->GetEnemyInSight())
	{
		int fleeProbability = Monster::s_chase_to_flee_or_chase[0];
		int chaseProbability = Monster::s_chase_to_flee_or_chase[1];

		int weight = rand() % WEIGHTING_MAX + WEIGHTING_MIN;

		if (weight <= WEIGHTING_MAX && weight > WEIGHTING_MAX - fleeProbability)
		{
			monster->SetPathFound(false);

			if (Monster::s_chase_to_flee_or_chase[0] >= WEIGHTING_MIN)
			{
				Monster::s_chase_to_flee_or_chase[0]--;
			}

			if (Monster::s_chase_to_flee_or_chase[1] <= WEIGHTING_MAX)
			{
				Monster::s_chase_to_flee_or_chase[1]++;
			}

			return FLEE;
		}
		else if (weight > WEIGHTING_MIN + (chaseProbability - 1) && weight < WEIGHTING_MIN)
		{
			if (Monster::s_chase_to_flee_or_chase[0] >= WEIGHTING_MIN)
			{
				Monster::s_chase_to_flee_or_chase[0]--;
			}

			if (Monster::s_chase_to_flee_or_chase[1] <= WEIGHTING_MAX)
			{
				Monster::s_chase_to_flee_or_chase[1]++;
			}

			return CHASE;
		}
	}

	return CHASE;
}

State StateMachine::IsSearching(Player* player, Monster* monster, float deltaTime)
{
	if (monster->GetPlayerInSight() && !player->GetEnemyInSight())
	{
		int sneakProbability = Monster::s_search_to_sneak_or_chase[0];
		int chaseProbability = Monster::s_search_to_sneak_or_chase[1];

		int weight = rand() % WEIGHTING_MAX + WEIGHTING_MIN;

		if (weight <= WEIGHTING_MAX && weight > WEIGHTING_MAX - sneakProbability)
		{
			monster->SetPathFound(false);

			if (Monster::s_search_to_sneak_or_chase[0] >= WEIGHTING_MIN)
			{
				Monster::s_search_to_sneak_or_chase[0]--;
			}

			if (Monster::s_search_to_sneak_or_chase[1] <= WEIGHTING_MAX)
			{
				Monster::s_search_to_sneak_or_chase[1]++;
			}

			return SNEAK;
		}
		else if (weight <= WEIGHTING_MIN + (chaseProbability - 1) && weight >= WEIGHTING_MIN)
		{
			monster->SetPathFound(false);

			if (Monster::s_search_to_sneak_or_chase[0] >= WEIGHTING_MIN)
			{
				Monster::s_search_to_sneak_or_chase[0]--;
			}

			if (Monster::s_search_to_sneak_or_chase[1] <= WEIGHTING_MAX)
			{
				Monster::s_search_to_sneak_or_chase[1]++;
			}

			return CHASE;
		}
	}
	else if (monster->GetPlayerInSight() && player->GetEnemyInSight())
	{
		int chaseProbability = Monster::s_search_to_chase_or_flee[0];
		int fleeProbability = Monster::s_search_to_chase_or_flee[1];

		int weight = rand() % 100 + 1;

		if (weight <= WEIGHTING_MIN + (chaseProbability - 1) && weight >= WEIGHTING_MIN)
		{
			monster->SetPathFound(false);

			if (Monster::s_search_to_chase_or_flee[1] >= WEIGHTING_MIN)
			{
				Monster::s_search_to_chase_or_flee[1]--;
			}

			if (Monster::s_search_to_chase_or_flee[0] <= WEIGHTING_MAX)
			{
				Monster::s_search_to_chase_or_flee[0]++;
			}

			return CHASE;
		}
		else if (weight <= WEIGHTING_MAX && weight > WEIGHTING_MAX - fleeProbability)
		{
			monster->SetPathFound(false);

			if (Monster::s_search_to_chase_or_flee[1] >= WEIGHTING_MIN)
			{
				Monster::s_search_to_chase_or_flee[1]--;
			}

			if (Monster::s_search_to_chase_or_flee[0] <= WEIGHTING_MAX)
			{
				Monster::s_search_to_chase_or_flee[0]++;
			}

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
	int fleeProbability = Monster::s_sneak_to_chase_or_flee[1];
	int chaseProbability = Monster::s_sneak_to_chase_or_flee[0];

	int weight = rand() % 100 + 1;

	if (monster->GetPlayerInSight() && player->GetEnemyInSight())
	{
		if (weight <= WEIGHTING_MAX && weight > WEIGHTING_MAX - fleeProbability)
		{
			monster->SetPathFound(false);

			if (Monster::s_sneak_to_chase_or_flee[1] >= WEIGHTING_MIN)
			{
				Monster::s_sneak_to_chase_or_flee[1]--;
			}

			if (Monster::s_sneak_to_chase_or_flee[0] <= WEIGHTING_MAX)
			{
				Monster::s_sneak_to_chase_or_flee[0]++;
			}

			return FLEE;
		}
		else if (weight >= WEIGHTING_MIN + (chaseProbability - 1) && weight > WEIGHTING_MIN)
		{
			if (Monster::s_sneak_to_chase_or_flee[1] >= WEIGHTING_MIN)
			{
				Monster::s_sneak_to_chase_or_flee[1]--;
			}

			if (Monster::s_sneak_to_chase_or_flee[0] <= WEIGHTING_MAX)
			{
				Monster::s_sneak_to_chase_or_flee[0]++;
			}

			return CHASE;
		}
	}
	else if (!monster->GetPlayerInSight())
	{
		//search or random
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
			monster->SetSoundHeard(false);
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

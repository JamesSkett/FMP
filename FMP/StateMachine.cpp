#include "StateMachine.h"
#include "Math.h"
#include "Monster.h"
#include "Player.h"

bool StateMachine::s_chasing = false;

StateMachine::StateMachine()
{
}

StateMachine::~StateMachine()
{
}

void StateMachine::RunStateMachine(Player* player, Monster* monster, float deltaTime)
{
	//check what the current state is
	switch (m_currentState)
	{
	case RANDOM_WANDER: //if random wander
		m_currentStateText = "Random Wander";             //set the current state text
		monster->RandomWander(deltaTime);                 //call the random wander behaviour
		m_currentState = IsRandomWander(player, monster); //check the conditions of this state
		break;
	case CHASE: //if chasing
		m_currentStateText = "Chase";							//set the current state text
		monster->Chase(deltaTime);								//call the chase behaviour
		m_currentState = IsChasing(player, monster, deltaTime); //check the conditions of this state
		break;
	case SEARCH: //if searching
		m_currentStateText = "Search";												//set the current state text
		monster->Search(XMFLOAT2(player->GetXPos(), player->GetYPos()), deltaTime);	//call the search behaviour
		m_currentState = IsSearching(player, monster, deltaTime);					//check the conditions of this state
		break;
	case SNEAK:
		m_currentStateText = "Sneak";							 //set the current state text
		monster->Sneak(player, deltaTime);						 //call the sneak behaviour
		m_currentState = IsSneaking(player, monster, deltaTime); //check the conditions of this state
		break;
	case FLEE:
		m_currentStateText = "Flee";							//set the current state text
		monster->Flee(player, deltaTime);						//call the flee behaviour
		m_currentState = IsFleeing(player, monster, deltaTime);	//check the conditions of this state
		break;
	}
}

void StateMachine::SetCurrentState(State state)
{
	m_currentState = state;
}

string StateMachine::GetCurrentState() const
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
				Monster::s_random_to_chase_or_sneak[1] -= weightChange;
			}

			if (Monster::s_random_to_chase_or_sneak[0] <= WEIGHTING_MAX)
			{
				Monster::s_random_to_chase_or_sneak[0] += weightChange;
			}

			
			char s[128];
			sprintf_s(s, "random to chase or sneak %d, %d\n\n", Monster::s_random_to_chase_or_sneak[0], Monster::s_random_to_chase_or_sneak[1]);
			OutputDebugString(s);

			return SNEAK;
		}
		else if (weight <= (WEIGHTING_MIN + chaseProbability - 1) && weight >= WEIGHTING_MIN)
		{
			monster->SetPathFound(false);
			s_chasing = true;

			if (Monster::s_random_to_chase_or_sneak[1] >= WEIGHTING_MIN)
			{
				Monster::s_random_to_chase_or_sneak[1] -= weightChange;
			}

			if (Monster::s_random_to_chase_or_sneak[0] <= WEIGHTING_MAX)
			{
				Monster::s_random_to_chase_or_sneak[0] += weightChange;
			}

			char s[128];
			sprintf_s(s, "random to chase or sneak %d, %d\n\n", Monster::s_random_to_chase_or_sneak[0], Monster::s_random_to_chase_or_sneak[1]);
			OutputDebugString(s);

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
				Monster::s_random_to_chase_or_flee[1] -= weightChange;
			}

			if (Monster::s_random_to_chase_or_flee[0] <= WEIGHTING_MAX)
			{
				Monster::s_random_to_chase_or_flee[0] += weightChange;
			}

			char s[128];
			sprintf_s(s, "random to chase or flee %d, %d\n\n", Monster::s_random_to_chase_or_flee[0], Monster::s_random_to_chase_or_flee[1]);
			OutputDebugString(s);

			return CHASE;
		}
		else if (weight <= WEIGHTING_MAX && weight > WEIGHTING_MAX - fleeProbability)
		{
			monster->SetPathFound(false);

			if (Monster::s_random_to_chase_or_flee[1] >= WEIGHTING_MIN)
			{
				Monster::s_random_to_chase_or_flee[1] -= weightChange;
			}

			if (Monster::s_random_to_chase_or_flee[0] <= WEIGHTING_MAX)
			{
				Monster::s_random_to_chase_or_flee[0] += weightChange;
			}

			char s[128];
			sprintf_s(s, "random to chase or flee %d, %d\n\n", Monster::s_random_to_chase_or_flee[0], Monster::s_random_to_chase_or_flee[1]);
			OutputDebugString(s);

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
				Monster::s_chase_to_search_or_random[1] -= weightChange;
			}

			if (Monster::s_chase_to_search_or_random[0] <= WEIGHTING_MAX)
			{
				Monster::s_chase_to_search_or_random[0] += weightChange;
			}

			char s[128];
			sprintf_s(s, "chase to search or random %d, %d\n\n", Monster::s_chase_to_search_or_random[0], Monster::s_chase_to_search_or_random[1]);
			OutputDebugString(s);

			return SEARCH;
		}
		else if (weight <= WEIGHTING_MAX && weight > WEIGHTING_MAX - randomWandProbability)
		{
			monster->SetPathFound(false);

			if (Monster::s_chase_to_search_or_random[1] >= WEIGHTING_MIN)
			{
				Monster::s_chase_to_search_or_random[1] -= weightChange;
			}

			if (Monster::s_chase_to_search_or_random[0] <= WEIGHTING_MAX)
			{
				Monster::s_chase_to_search_or_random[0] += weightChange;
			}

			char s[128];
			sprintf_s(s, "chase to search or random %d, %d\n\n", Monster::s_chase_to_search_or_random[0], Monster::s_chase_to_search_or_random[1]);
			OutputDebugString(s);

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
				Monster::s_chase_to_flee_or_chase[0] -= weightChange;
			}

			if (Monster::s_chase_to_flee_or_chase[1] <= WEIGHTING_MAX)
			{
				Monster::s_chase_to_flee_or_chase[1] += weightChange;
			}

			char s[128];
			sprintf_s(s, "chase to flee or chase %d, %d\n\n", Monster::s_chase_to_flee_or_chase[0], Monster::s_chase_to_flee_or_chase[1]);
			OutputDebugString(s);

			return FLEE;
		}
		else if (weight > WEIGHTING_MIN + (chaseProbability - 1) && weight < WEIGHTING_MIN)
		{
			if (Monster::s_chase_to_flee_or_chase[0] >= WEIGHTING_MIN)
			{
				Monster::s_chase_to_flee_or_chase[0] -= weightChange;
			}

			if (Monster::s_chase_to_flee_or_chase[1] <= WEIGHTING_MAX)
			{
				Monster::s_chase_to_flee_or_chase[1] += weightChange;
			}

			char s[128];
			sprintf_s(s, "chase to flee or chase %d, %d\n\n", Monster::s_chase_to_flee_or_chase[0], Monster::s_chase_to_flee_or_chase[1]);
			OutputDebugString(s);

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
				Monster::s_search_to_sneak_or_chase[0] -= weightChange;
			}

			if (Monster::s_search_to_sneak_or_chase[1] <= WEIGHTING_MAX)
			{
				Monster::s_search_to_sneak_or_chase[1] += weightChange;
			}

			char s[128];
			sprintf_s(s, "search to sneak or chase %d, %d\n\n", Monster::s_search_to_sneak_or_chase[0], Monster::s_search_to_sneak_or_chase[1]);
			OutputDebugString(s);

			return SNEAK;
		}
		else if (weight <= WEIGHTING_MIN + (chaseProbability - 1) && weight >= WEIGHTING_MIN)
		{
			monster->SetPathFound(false);

			if (Monster::s_search_to_sneak_or_chase[0] >= WEIGHTING_MIN)
			{
				Monster::s_search_to_sneak_or_chase[0] -= weightChange;
			}

			if (Monster::s_search_to_sneak_or_chase[1] <= WEIGHTING_MAX)
			{
				Monster::s_search_to_sneak_or_chase[1] += weightChange;
			}

			char s[128];
			sprintf_s(s, "search to sneak or chase %d, %d\n\n", Monster::s_search_to_sneak_or_chase[0], Monster::s_search_to_sneak_or_chase[1]);
			OutputDebugString(s);

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
				Monster::s_search_to_chase_or_flee[1] -= weightChange;
			}

			if (Monster::s_search_to_chase_or_flee[0] <= WEIGHTING_MAX)
			{
				Monster::s_search_to_chase_or_flee[0] += weightChange;
			}

			char s[128];
			sprintf_s(s, "search to chase or flee %d, %d\n\n", Monster::s_search_to_chase_or_flee[0], Monster::s_search_to_chase_or_flee[1]);
			OutputDebugString(s);

			return CHASE;
		}
		else if (weight <= WEIGHTING_MAX && weight > WEIGHTING_MAX - fleeProbability)
		{
			monster->SetPathFound(false);

			if (Monster::s_search_to_chase_or_flee[1] >= WEIGHTING_MIN)
			{
				Monster::s_search_to_chase_or_flee[1] -= weightChange;
			}

			if (Monster::s_search_to_chase_or_flee[0] <= WEIGHTING_MAX)
			{
				Monster::s_search_to_chase_or_flee[0] += weightChange;
			}

			char s[128];
			sprintf_s(s, "search to chase or flee %d, %d\n\n", Monster::s_search_to_chase_or_flee[0], Monster::s_search_to_chase_or_flee[1]);
			OutputDebugString(s);

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

State StateMachine::IsSneaking(Player* player, Monster* monster, float deltaTime)
{
	if (monster->GetPlayerInSight() && player->GetEnemyInSight())
	{
		

		int fleeProbability = Monster::s_sneak_to_chase_or_flee[1];
		int chaseProbability = Monster::s_sneak_to_chase_or_flee[0];

		int weight = rand() % 100 + 1;

		if (weight <= WEIGHTING_MAX && weight > WEIGHTING_MAX - fleeProbability)
		{
			monster->SetPathFound(false);

			if (Monster::s_sneak_to_chase_or_flee[1] >= WEIGHTING_MIN)
			{
				Monster::s_sneak_to_chase_or_flee[1] -= weightChange;
			}

			if (Monster::s_sneak_to_chase_or_flee[0] <= WEIGHTING_MAX)
			{
				Monster::s_sneak_to_chase_or_flee[0] += weightChange;
			}

			char s[128];
			sprintf_s(s, "sneak to chase or flee %d, %d\n\n", Monster::s_sneak_to_chase_or_flee[0], Monster::s_sneak_to_chase_or_flee[1]);
			OutputDebugString(s);

			return FLEE;
		}
		else if (weight >= WEIGHTING_MIN + (chaseProbability - 1) && weight > WEIGHTING_MIN)
		{
			if (Monster::s_sneak_to_chase_or_flee[1] >= WEIGHTING_MIN)
			{
				Monster::s_sneak_to_chase_or_flee[1] -= weightChange;
			}

			if (Monster::s_sneak_to_chase_or_flee[0] <= WEIGHTING_MAX)
			{
				Monster::s_sneak_to_chase_or_flee[0] += weightChange;
			}

			char s[128];
			sprintf_s(s, "sneak to chase or flee %d, %d\n\n", Monster::s_sneak_to_chase_or_flee[0], Monster::s_sneak_to_chase_or_flee[1]);
			OutputDebugString(s);

			return CHASE;
		}
	}
	else if (monster->GetPlayerInSight() && !player->GetEnemyInSight())
	{
		if (m_attackTimer <= 0)
		{
			m_attackTimer = 1.0f;
			return CHASE;
		}
		m_attackTimer -= deltaTime;
	}
	else if (!monster->GetPlayerInSight())
	{
		int randomProbability = Monster::s_sneak_to_search_or_random[1];
		int searchProbability = Monster::s_sneak_to_search_or_random[0];

		int weight = rand() % 100 + 1;

		if (m_sneakTimer <= 0)
		{
			if (weight <= WEIGHTING_MAX && weight > WEIGHTING_MAX - randomProbability)
			{
				monster->SetPathFound(false);

				if (Monster::s_sneak_to_search_or_random[1] >= WEIGHTING_MIN)
				{
					Monster::s_sneak_to_search_or_random[1] -= weightChange;
				}

				if (Monster::s_sneak_to_search_or_random[0] <= WEIGHTING_MAX)
				{
					Monster::s_sneak_to_search_or_random[0] += weightChange;
				}

				m_sneakTimer = 1.0f;

				char s[128];
				sprintf_s(s, "sneak to search or random %d, %d\n\n", Monster::s_sneak_to_search_or_random[0], Monster::s_sneak_to_search_or_random[1]);
				OutputDebugString(s);

				return RANDOM_WANDER;
			}
			else if (weight >= WEIGHTING_MIN + (searchProbability - 1) && weight > WEIGHTING_MIN)
			{
				if (Monster::s_sneak_to_search_or_random[1] >= WEIGHTING_MIN)
				{
					Monster::s_sneak_to_search_or_random[1] -= weightChange;
				}

				if (Monster::s_sneak_to_search_or_random[0] <= WEIGHTING_MAX)
				{
					Monster::s_sneak_to_search_or_random[0] += weightChange;
				}

				m_sneakTimer = 1.0f;

				char s[128];
				sprintf_s(s, "sneak to search or random %d, %d\n\n", Monster::s_sneak_to_search_or_random[0], Monster::s_sneak_to_search_or_random[1]);
				OutputDebugString(s);

				return SEARCH;
			}
		}
		m_sneakTimer -= deltaTime;
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

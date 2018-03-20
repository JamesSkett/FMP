#include "Pathfinding.h"
#include <math.h>
#include <algorithm>

Pathfinding::Pathfinding(vector <Tile*> tilemap)
{
	for (unsigned int i = 0; i < tilemap.size(); i++)
	{
		m_nodeList.push_back(new Node(i, tilemap[i]->GetIsWalkable(), tilemap[i]->GetXPos(), tilemap[i]->GetYPos()));
	}
}


Pathfinding::~Pathfinding()
{
	for (unsigned int i = 0; i < m_openList.size(); i++)
	{
		delete m_openList[i];
		m_openList[i] = nullptr;
	}

	m_openList.clear();

	for (unsigned int i = 0; i < m_closedList.size(); i++)
	{
		delete m_closedList[i];
		m_closedList[i] = nullptr;
	}

	m_closedList.clear();

	for (unsigned int i = 0; i < m_nodeList.size(); i++)
	{
		delete m_nodeList[i];
		m_nodeList[i] = nullptr;
	}

	m_nodeList.clear();
}

void Pathfinding::FindPath(XMFLOAT2 startPos, XMFLOAT2 targetPos)
{
	//list of nodes that form the path
	vector <XMFLOAT2> waypoints;
	//lets us know if we found a path
	bool pathSuccess = false;

	//get the start and destination nodes from the grid
	Node* startNode = GetNodeFromList(startPos);
	Node* targetNode = GetNodeFromList(targetPos);

	//check that the starting and ending nodes are valid
	if (startNode->walkable && targetNode->walkable)
	{
		//add the start node to the open list
		AddToOpenList(startNode);

		//While there are still nodes to evaluate
		while (m_openList.size() > 0)
		{
			Node* currentNode = m_openList.back();
			m_openList.erase(m_openList.end());

			m_closedList.push_back(currentNode);

			if (currentNode == targetNode)
			{
				pathSuccess = true;
				break;
			}

			for each (Node* neighbour in GetNeighbours(currentNode))
			{
				if (!neighbour->walkable || find(m_closedList.begin(), m_closedList.end(), neighbour) != m_closedList.end())
				{
					continue;
				}

				int newMovementCostToNeigbour = currentNode->gValue + m_movementCost;

				if (newMovementCostToNeigbour < neighbour->gValue || find(m_openList.begin(), m_openList.end(), neighbour) != m_openList.end())
				{
					//Store the new costs
					neighbour->gValue = newMovementCostToNeigbour;
					neighbour->hValue = CalculateHValue(neighbour, targetNode);
					neighbour->parent = currentNode;

					if (find(m_closedList.begin(), m_closedList.end(), neighbour) != m_closedList.end())
					{
						AddToOpenList(neighbour);
					}
				}
			}
		}
	}
}

void Pathfinding::AddToOpenList(Node* node)
{
	if (m_openList.empty())
	{
		m_openList.push_back(node);
	}
	else
	{
		bool placed = false;

		for (int i = 0; i < m_openList.size(); i++)
		{
			if (node->fCost < m_openList[i]->fCost)
			{
				m_openList.insert(m_closedList.begin() + i, node);
				placed = true;
				break;
			}
		}

		if (!placed)
		{
			m_openList.push_back(node);
		}
	}

}

void Pathfinding::AddToClosedList(Node* node)
{
	m_closedList.push_back(node);
}

vector <Node*> Pathfinding::GetNeighbours(Node* currentNode)
{
	vector <Node*> neigbours;

	float currentX = currentNode->xPos;
	float currentY = currentNode->yPos;

	for (unsigned int i = 0; i < m_nodeList.size(); i++)
	{
		float checkNodeX = m_nodeList[i]->xPos;
		float checkNodeY = m_nodeList[i]->yPos;

		float distance = ((checkNodeX - currentX) * (checkNodeX - currentX)) + ((checkNodeY - currentY) * (checkNodeY - currentY));

		if (distance == 0) continue;
		

		if (distance < 0.4f && distance > -0.4)
		{
			neigbours.push_back(m_nodeList[i]);
		}
	}

	return neigbours;
}

int Pathfinding::CalculateHValue(Node* n1, Node* n2)
{
	
	int h = abs(n1->xPos - n2->xPos) + abs(n1->yPos - n2->yPos);
		
	return h;
}

Node * Pathfinding::GetNodeFromList(XMFLOAT2 pos)
{
	float currentDistance;
	float minDistance = pow(m_nodeList[0]->xPos - pos.x, 2) + pow(m_nodeList[0]->yPos - pos.y, 2);
	Node* currentNode;
	for (int i = 0; i < m_nodeList.size(); i++)
	{
		float startPosX = pos.x;
		float startPosY = pos.y;
		float nodePosX = m_nodeList[i]->xPos;
		float nodePosY = m_nodeList[i]->yPos;

		currentDistance = ((nodePosX - startPosX) * (nodePosX - startPosX)) + ((nodePosY - startPosY) * (nodePosY - startPosY));

		if (currentDistance < minDistance)
		{
			minDistance = currentDistance;
			currentNode = m_nodeList[i];
		}
	}

	return currentNode;
}

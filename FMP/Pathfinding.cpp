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

vector <XMFLOAT2> Pathfinding::FindPath(XMFLOAT2 startPos, XMFLOAT2 targetPos)
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
			//Get the node with the lowest cost
			Node* currentNode = m_openList.back();
			//remove it from the open list
			m_openList.erase(m_openList.end() - 1);
			//add it to the closed list
			m_closedList.push_back(currentNode);

			//Path has been found so set flag to true and exit
			if (currentNode == targetNode)
			{
				pathSuccess = true;
				break;
			}

			//evaluate all the neigbours
			for each (Node* neighbour in GetNeighbours(currentNode))
			{
				//check if the node we are looking at is navigable or already closed.
				if (!neighbour->walkable || find(m_closedList.begin(), m_closedList.end(), neighbour) != m_closedList.end())
				{
					//No point in evaluating it
					continue;
				}

				//Get the total cost for this neighbour
				int newMovementCostToNeigbour = currentNode->gValue + CalculateHValue(currentNode, targetNode);

				//If this neighbour has a lower cost from this node or is not in the in the open list evaluate it
				if (newMovementCostToNeigbour < neighbour->gValue || find(m_openList.begin(), m_openList.end(), neighbour) == m_openList.end())
				{
					//Store the new costs
					neighbour->gValue = newMovementCostToNeigbour;
					neighbour->hValue = CalculateHValue(neighbour, targetNode);
					neighbour->parent = currentNode;
					neighbour->fCost = neighbour->gValue + neighbour->hValue;

					//If this neighbour is not on the open list add it to the open list.
					if (find(m_openList.begin(), m_openList.end(), neighbour) == m_openList.end())
					{
						AddToOpenList(neighbour);
					}
				}
			}
		}

		//Build the path
		if (pathSuccess)
		{
			waypoints = RetracePath(startNode, targetNode);
			m_pathFound = true;
		}

		m_closedList.clear();
		m_openList.clear();

		return waypoints;
	}

	return vector<XMFLOAT2>();
}

vector<XMFLOAT2> Pathfinding::RetracePath(Node * startNode, Node * endNode)
{
	vector <XMFLOAT2> waypoints;

	Node* currentNode = endNode;

	while (currentNode != startNode)
	{
		waypoints.push_back(XMFLOAT2(currentNode->xPos, currentNode->yPos));
		currentNode = currentNode->parent;
	}


	reverse(waypoints.begin(), waypoints.end());

	return waypoints;
}

void Pathfinding::AddToOpenList(Node* node)
{
	//sorts the open list in order of fCost
	if (m_openList.empty())
	{
		m_openList.push_back(node);
	}
	else
	{
		bool placed = false;

		for (unsigned int i = 0; i < m_openList.size(); i++)
		{
			if (node->fCost > m_openList[i]->fCost)
			{
				m_openList.insert(m_openList.begin() + i, node);
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
		if (currentNode->index == m_nodeList[i]->index)
		{
			continue;
		}
		
		if (m_nodeList[i]->index == currentNode->index - 41 || m_nodeList[i]->index == currentNode->index - 1 || m_nodeList[i]->index == currentNode->index + 1 || m_nodeList[i]->index == currentNode->index + 41)
		{
			neigbours.push_back(m_nodeList[i]);
		}


	}

	return neigbours;
}

int Pathfinding::CalculateHValue(Node* n1, Node* n2)
{
	
	int h = (int)abs(n1->xPos - n2->xPos) + (int)abs(n1->yPos - n2->yPos);
		
	return h;
}

Node * Pathfinding::GetNodeFromList(XMFLOAT2 pos)
{
	float currentDistance;
	float minDistance = pow(m_nodeList[0]->xPos - pos.x, 2) + pow(m_nodeList[0]->yPos - pos.y, 2);
	Node* currentNode = m_nodeList[0];
	for (unsigned int i = 0; i < m_nodeList.size(); i++)
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

bool Pathfinding::GetIsPathFound()
{
	return m_pathFound;
}

void Pathfinding::SetIsPathFound(bool pathFound)
{
	m_pathFound = pathFound;
}

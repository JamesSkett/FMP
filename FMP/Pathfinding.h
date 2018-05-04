/*
	Class:		Node
	Project:	Adaptive Monster AI for a Horror Game (Final Major Project)
	Author:		James Skett
	Purpose:	Node class is used for the open and closed lists in the pathfinding algorithm
				it stores the F, H and G values for each tile checked
*/

#pragma once
#include "Tile.h"
#include "Player.h"

#include <vector>

using namespace std;

//Node class to store the properties of each node
class Node
{
public:
	bool walkable;
	int index;
	int gValue = 1;
	int hValue;
	int fCost;

	float xPos, yPos;

	//each node has a parent so the path can be retraced
	Node* parent;

	Node::Node(int i, bool canWalk, float x, float y)
	{
		index = i;
		walkable = canWalk;
		xPos = x;
		yPos = y;
		parent = nullptr;
	}

	Node::~Node()
	{
		if (parent)
		{
			delete parent;
			parent = nullptr;
		}
	}

	
};

/*
	Class:		Pathfinding
	Project:	Adaptive Monster AI for a Horror Game (Final Major Project)
	Author:		James Skett
	Purpose:	This uses the A* pathfinding algorithm to find the optimal path to take to a specified location
*/

class Pathfinding
{
public:
	Pathfinding(vector <Tile*> tilemap);
	~Pathfinding();

	//calculates a path to given location
	vector <XMFLOAT2*> FindPath(XMFLOAT2 startPos, XMFLOAT2 targetPos);
	//retraces the path from end node to start node using the parents
	vector <XMFLOAT2*> RetracePath(Node* startNode, Node* endNode);

	//Adds a node to the open list
	void AddToOpenList(Node* node);
	//Adds a node to the closed list
	void AddToClosedList(Node* node);

	//Gets the nodes adjacent to the current node
	vector <Node*> GetNeighbours(Node* currentNode);

	//calculates the h value for 2 nodes
	int CalculateHValue(Node* n1, Node* n2);

	//Finds the node for the position passed in
	Node* GetNodeFromList(XMFLOAT2 pos);

	//get the value of m_pathFound
	bool GetIsPathFound();
	//set the value of m_pathFound
	void SetIsPathFound(bool pathFound);

private:
	vector <Node*> m_openList;
	vector <Node*> m_closedList;
	vector <Node*> m_nodeList;

	Node* m_StartNode;
	
	bool m_pathFound = false;

	int m_movementCost = 1;
};


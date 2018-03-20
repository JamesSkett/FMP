#pragma once
#include "Tile.h"
#include "Monster.h"
#include "Player.h"

#include <vector>

using namespace std;

class Node
{
public:
	bool walkable;
	int index;
	int gValue;
	int hValue;
	int fCost;

	float xPos, yPos;

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

class Pathfinding
{
public:
	Pathfinding(vector <Tile*> tilemap);
	~Pathfinding();

	void FindPath(XMFLOAT2 startPos, XMFLOAT2 targetPos);

	void AddToOpenList(Node* node);
	void AddToClosedList(Node* node);

	vector <Node*> GetNeighbours(Node* currentNode);

	int CalculateHValue(Node* n1, Node* n2);

	Node* GetNodeFromList(XMFLOAT2 pos);

private:
	vector <Node*> m_openList;
	vector <Node*> m_closedList;
	vector <Node*> m_nodeList;

	Node* m_StartNode;
	
	bool m_pathFound;

	int m_movementCost = 1;
};


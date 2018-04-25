#pragma once
#include "Tile.h"
#include "Player.h"

#include <vector>

using namespace std;

class Node
{
public:
	bool walkable;
	int index;
	int gValue = 1;
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

	vector <XMFLOAT2*> FindPath(XMFLOAT2 startPos, XMFLOAT2 targetPos);
	vector <XMFLOAT2*> RetracePath(Node* startNode, Node* endNode);

	void AddToOpenList(Node* node);
	void AddToClosedList(Node* node);

	vector <Node*> GetNeighbours(Node* currentNode);

	int CalculateHValue(Node* n1, Node* n2);

	Node* GetNodeFromList(XMFLOAT2 pos);

	bool GetIsPathFound();
	void SetIsPathFound(bool pathFound);

private:
	vector <Node*> m_openList;
	vector <Node*> m_closedList;
	vector <Node*> m_nodeList;

	Node* m_StartNode;
	
	bool m_pathFound = false;

	int m_movementCost = 1;
};


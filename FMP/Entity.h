/*
	Class:		Entity
	Project:	Adaptive Monster AI for a Horror Game (Final Major Project)
	Author:		James Skett
	Purpose:	Used for creating the verticies for the entities in the game and hold the collision detection algorithm
*/

#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <dinput.h>
#include <sstream>

#include <DirectXMath.h>
using namespace DirectX;

#include <vector>
#include "Scene_Node.h"

class Tile;
class Asset;

class Entity : public Scene_Node
{
public:
	Entity(float x, float y, float z, float scale, float width, float height, float depth);
	~Entity();

	//Set the value of m_walkedThroughDoor
	void SetWalkedThroughDoor(bool value);

	void GetColBoxParameters(float &x, float &y, float &w, float &h);
	//Get the value of m_walkedThroughDoor
	bool GetWalkedThroughDoor();
	//Get the value of m_doorPos which is set in CollisionCheck()
	XMFLOAT2 GetDoorPos();
	//Check collitions with the walls //Returns true/false
	bool CollisionCheck(std::vector<Tile*> tilemap);
	//check collisions with the monster //Returns true/false
	bool CollisionCheck(Entity* colObject);
	//Set eh entities view cone
	void SetViewCone(Asset* viewCone);
	//reset the entities to their start pos
	void Reset();

	struct POS_COL_VERTEX
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Col;
	};

	struct ENTITY_CONSTANT_BUFFER
	{
		XMMATRIX WorldViewProjection;
		XMFLOAT2 playerPos;
		float playerRotation;
		float range;
		int fog;
		XMFLOAT3 packingBytes;
	};
	
protected:
	HRESULT CreateVertices(XMFLOAT4 colour);

	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pInputLayout;
	ID3D11Buffer*			m_pConstantBuffer0;


	XMFLOAT2 m_startPos;

	float m_screenSpaceX;
	float m_screenSpaceY;

	Asset* m_viewCone;

	bool m_walkedThroughDoor = false;
	XMFLOAT2 m_doorPos;

};




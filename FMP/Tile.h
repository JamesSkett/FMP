/*
	Class:		Tile
	Project:	Adaptive Monster AI for a Horror Game (Final Major Project)
	Author:		James Skett
	Purpose:	Creates and draws the vertices for all the tiles
*/

#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <dinput.h>
#include <sstream>

#include <DirectXMath.h>
using namespace DirectX;

#include "Scene_Node.h"

class Tile : public Scene_Node
{
public:
	Tile(float x, float y, float z, float scale, float width, float height, int index);
	~Tile();

	void Draw(XMMATRIX view, XMMATRIX projection);

	//get the position, width and height of the tile for collisions
	void GetParameters(float &x, float &y, float &width, float &height);
	//Get the index value of the current tile 1 = floor 2 = wall 3 = door
	int GetIndex();

	//check to see if the tile is walkable or not
	bool GetIsWalkable();

	struct POS_COL_VERTEX
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Col;
	};

	struct TILE_CONSTANT_BUFFER
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

	float m_w, m_h;

	bool m_walkable;

	int m_index;

};




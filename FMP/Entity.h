#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <dinput.h>
#include <sstream>

#include <DirectXMath.h>
using namespace DirectX;

#include <vector>

using namespace std;

#include "Tile.h"
class Asset;

class Entity
{
public:
	Entity(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height);
	~Entity();

	void Draw(XMMATRIX view, XMMATRIX projection);

	//Set the value of m_walkedThroughDoor
	void SetWalkedThroughDoor(bool value);

	void GetColBoxParameters(float &x, float &y, float &w, float &h);
	//Get the value of m_walkedThroughDoor
	bool GetWalkedThroughDoor();
	//Get the value of m_doorPos which is set in CollisionCheck()
	XMFLOAT2 GetDoorPos();
	//Check collitions with the walls //Returns true/false
	bool CollisionCheck(vector<Tile*> tilemap);
	//check collisions with the monster //Returns true/false
	bool CollisionCheck(Entity * colObject);

	void SetViewCone(Asset* viewCone);

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

	float m_xPos, m_yPos, m_zPos, m_scale;
	float m_rotation;
	float m_width, m_height;

	float m_dirX = 0.0f;
	float m_dirY = 0.0f;

	float m_screenSpaceX;
	float m_screenSpaceY;

	Asset* m_viewCone;

	bool m_walkedThroughDoor = false;
	XMFLOAT2 m_doorPos;

};




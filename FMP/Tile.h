#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <dinput.h>
#include <sstream>

#include <DirectXMath.h>
using namespace DirectX;

class Tile
{
public:
	Tile(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height);
	~Tile();

	void Draw(XMMATRIX view, XMMATRIX projection);

	void GetParameters(float &x, float &y, float &width, float &height);
	int GetIndex();

	float GetXPos();
	float GetYPos();

	bool GetIsWalkable();

	void SetGValue(int value);
	void SetHValue(int value);
	void SetFValue(int value);

	int Get_G_Value();
	int Get_H_Value();
	int Get_F_Value();

	struct POS_COL_VERTEX
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Col;
	};

	struct ENTITY_CONSTANT_BUFFER
	{
		XMMATRIX WorldViewProjection;
	};

protected:
	HRESULT CreateVertices(XMFLOAT4 colour);

	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pInputLayout;
	ID3D11Buffer*			m_pConstantBuffer0;

	float m_xPos, m_yPos, m_zPos, m_scale;
	float m_xAngle, m_yAngle, m_zAngle;

	float m_w, m_h;

	bool m_walkable;

	int m_index;

	int m_gValue;
	int m_hValue;
	int m_fValue;
};




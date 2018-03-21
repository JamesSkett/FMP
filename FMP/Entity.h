#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <dinput.h>
#include <sstream>

#include <DirectXMath.h>
using namespace DirectX;

class Entity
{
public:
	Entity(XMFLOAT4 colour, float x, float y, float z, float scale, float width, float height);
	~Entity();

	void Draw(XMMATRIX view, XMMATRIX projection);

	void GetColBoxParameters(float &x, float &y, float &w, float &h);

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
	float m_width, m_height;

	float m_dirX = 0.0f;
	float m_dirY = 0.0f;
};




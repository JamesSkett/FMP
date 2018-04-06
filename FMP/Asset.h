#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <dinput.h>
#include <sstream>

#include <DirectXMath.h>
using namespace DirectX;

class Asset
{
public:
	Asset(char* colour, float x, float y, float z, float scale, float width, float height);
	~Asset();

	void Draw(XMMATRIX view, XMMATRIX projection);

	void SetPos(float x, float y);
	void SetRotation(float rotation);

	struct POS_TEX_VERTEX
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
	};

	struct ENTITY_CONSTANT_BUFFER
	{
		XMMATRIX WorldViewProjection;
	};

private:
	HRESULT CreateVertices(char* filename);

	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pInputLayout;
	ID3D11Buffer*			m_pConstantBuffer0;

	ID3D11ShaderResourceView* m_pTexture;
	ID3D11SamplerState*       m_pSampler0;

	float m_xPos, m_yPos, m_zPos, m_scale;
	float m_rotation;
	float m_width, m_height;

	float m_dirX = 0.0f;
	float m_dirY = 0.0f;

	float m_screenSpaceX;
	float m_screenSpaceY;
};


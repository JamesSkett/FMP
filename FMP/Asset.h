#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <dinput.h>
#include <sstream>

#include <DirectXMath.h>
using namespace DirectX;

#include "Renderer.h"

class Asset
{
public:
	Asset(char* colour, float x, float y, float z, float scale, float width, float height, float rotation);
	~Asset();

	void Draw(XMMATRIX view, XMMATRIX projection);

	//Set the position of the asset
	void SetPos(float x, float y);
	//Set the rotation value for the asset
	void SetRotation(float rotation);
	//Set the scale of the asset
	void SetScale(float value);
	//Set the colour values for the asset
	void SetColour(XMFLOAT4 colour);
	//Set whether the asset can be drawn or not
	void SetCanDraw(bool canDraw);


	void UpdateRotation(float speed);
	void UpdateScale(float speed);

	void GetParameters(float &x, float &y, float &w, float &h);
	bool GetCanDraw();
	float GetScale();

	XMFLOAT2 GetPos();

	struct POS_TEX_VERTEX
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
	};

	struct ENTITY_CONSTANT_BUFFER
	{
		XMMATRIX WorldViewProjection;
		XMFLOAT4 colour;
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

	XMFLOAT4 m_colour;

	bool m_canDraw = true;
};


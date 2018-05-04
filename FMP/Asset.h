/*
		Class:		Asset
		Project:	Adaptive Monster AI for a Horror Game (Final Major Project)
		Author:		James Skett
		Purpose:	Creates in game assets that have textures applied to them also supports transparent images
*/

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
	Asset(const char* filename, float x, float y, float z, float scale, float rotation);
	Asset(const char* filename, float x, float y, float z, float scale, float width, float height, float rotation);
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

	//Update the rotation value
	void UpdateRotation(float speed);
	//upadate the scalar value
	void UpdateScale(float speed);

	//Get the posisiton and width and height values
	void GetParameters(float &x, float &y, float &w, float &h);
	//Get the value of m_canDraw
	bool GetCanDraw();
	//get the value of m_scale
	float GetScale();

	//Get the x and y values of the asset
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
	HRESULT CreateVertices(const char* filename);

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


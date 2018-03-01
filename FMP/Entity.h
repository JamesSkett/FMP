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
	Entity();
	~Entity();

	void Draw();
	struct POS_COL_VERTEX
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Col;
	};
private:
	HRESULT CreateVertices();

	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pInputLayout;
	ID3D11Buffer*			m_pConstantBuffer0;
};


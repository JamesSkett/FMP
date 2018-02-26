#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <dinput.h>
#include <sstream>

#include <DirectXMath.h>
using namespace DirectX;


class Renderer
{
public:
	Renderer();
	~Renderer();

	//DirectX Setup
	HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitialiseD3D();
	//Set up the graphics
	HRESULT InitialiseGraphics(void);

	//delete objects from memory before exiting
	void ShutdownD3D();

	//render objects in game
	void RenderFrame();

	static ID3D11Device*           m_pD3DDevice;
	static ID3D11DeviceContext*    m_pImmediateContext;

private:
	const float m_screenWidth = 1280.0f;
	const float m_screenHeight = 720.0f;

	char		m_GameName[100] = "FMP\0";


	HINSTANCE	m_hInst = NULL;

	//Drivers
	D3D_DRIVER_TYPE			m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	IDXGISwapChain*         m_pSwapChain = NULL;

	//Shader and depth view variables
	ID3D11RenderTargetView* m_pBackBufferRTView = NULL;

	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pInputLayout;
	ID3D11Buffer*			m_pConstantBuffer0;

	ID3D11DepthStencilView* m_pzBuffer;
	ID3D11ShaderResourceView* m_pTexture0;
	ID3D11SamplerState* m_pSampler0;
};


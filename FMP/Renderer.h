#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <dinput.h>
#include <sstream>


#include <DirectXMath.h>
using namespace DirectX;

#include "CXBOXController.h"
#include "text2D.h"
#include "Time.h"
#include "Entity.h"
#include "Level.h"
#include "Colours.h"



__declspec(align(16)) class Renderer
{
public:
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		return _mm_free(p);
	}

	Renderer();
	~Renderer();

	//DirectX Setup
	HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitialiseD3D();

	//delete objects from memory before exiting
	void ShutdownD3D();

	//render objects in game
	void RenderFrame();

	//Set up the graphics
	HRESULT InitialiseGraphics(void);

	//set up the keyboard and mouse input
	HRESULT InitialiseInput();
	void ReadInputState();
	bool IsKeyPressed(unsigned char DI_keycode);
	DIMOUSESTATE mouseCurrState;

	//These can be used in any class
	static Time time;
	static ID3D11Device*           pD3DDevice;
	static ID3D11DeviceContext*    pImmediateContext;
	static ID3D11RenderTargetView* pBackBufferRTView;
	static ID3D11BlendState*       pAlphaBlendEnable;
	static ID3D11BlendState*       pAlphaBlendDisable;
	static Colour colour;
	
	//matrices for object transforms
	XMMATRIX identity, projection, view;

	//Destroys the program window
	void DestroyWin();

	struct POS_COL_VERTEX
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Col;
	};

	struct CONSTANT_BUFFER0
	{
		XMMATRIX WorldViewProjection;
		float RedAmount;  //4 bytes
						  //float scale;
		XMFLOAT3 packing_bytes; // 3x4 bytes = 12 bytes
	};

private:
	
	//screen properties
	const float m_screenWidth = 1920.0f;
	const float m_screenHeight = 1080.0f;
	const float m_cNearClip = 0.1f;
	const float m_cFarClip = 100.0f;
	const float m_cOrthographicSize = 100.0f;

	//Name of the Game
	char		m_GameName[100] = "FMP\0";

	HINSTANCE	m_hInst = NULL;

	//Drivers
	D3D_DRIVER_TYPE			m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	IDXGISwapChain*         m_pSwapChain = NULL;

	//Shader and depth view variables

	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pInputLayout;
	ID3D11Buffer*			m_pConstantBuffer0;

	ID3D11DepthStencilView* m_pzBuffer;
	ID3D11ShaderResourceView* m_pTexture0;
	ID3D11SamplerState* m_pSampler0;
	


	//keyboard and mouse variables
	IDirectInput8* m_direct_input;
	IDirectInputDevice8* m_keyboard_device;
	IDirectInputDevice8* m_mouse_device;
	unsigned char m_keyboard_keys_state[256];

	//Frame counter variables
	Text2D* m_FPSCount;
	int m_fps = 0;

	//Entity* entity1;

};


/*		
		Class:		Renderer
		Project:	Adaptive Monster AI for a Horror Game (Final Major Project)
		Author:		James Skett
		Purpose:	Sets up the main directX 11 window and creates the keyboard and mouse devices
*/


#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <dinput.h>
#include <sstream>


#include <DirectXMath.h>
using namespace DirectX;

#include "Colours.h"

class Camera;
class CXBOXController;
class text2D;
class Time;
class Entity;
class Level;


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
	DIMOUSESTATE mouseLastState;

	//These can be used in any class
	static Camera* camera;
	static Time time;
	static ID3D11Device*           pD3DDevice;
	static ID3D11DeviceContext*    pImmediateContext;
	static ID3D11RenderTargetView* pBackBufferRTView;
	static ID3D11BlendState*       pAlphaBlendEnable;
	static ID3D11BlendState*       pAlphaBlendDisable;
	static ID3D11DepthStencilView* m_pzBuffer;
	static Colour colour;
	static int s_FogOfWar;

	//Destroys the program window
	void DestroyWin();

private:

	//screen properties
	const float m_screenWidth = 1920.0f;
	const float m_screenHeight = 1080.0f;
	const float m_cNearClip = 0.1f;
	const float m_cFarClip = 100.0f;
	const float m_cOrthographicSize = 100.0f;

	//Name of the Game
	char		m_GameName[100] = "Adaptive Monster AI\0";

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

	ID3D11ShaderResourceView* m_pTexture0;
	ID3D11SamplerState* m_pSampler0;



	//keyboard and mouse variables
	IDirectInput8* m_direct_input;
	IDirectInputDevice8* m_keyboard_device;
	IDirectInputDevice8* m_mouse_device;
	unsigned char m_keyboard_keys_state[256];

};


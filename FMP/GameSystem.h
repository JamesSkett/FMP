#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>

#include <DirectXMath.h>
using namespace DirectX;

#include <vector>

#include "Renderer.h"
#include "text2D.h"
#include "Level.h"
#include "Player.h"
#include "Time.h"

class GameSystem
{
public:
	GameSystem();
	~GameSystem();

	//play the main game loop
	int playGame(MSG msg, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	
	//set up all the objects in the game
	void SetupLevel();

	//get device input
	void GetKeyboardInput();
	void GetControllerInput();
	CXBOXController* player1 = new CXBOXController(1);

	void DrawLevel(XMMATRIX view, XMMATRIX projection);


private:
	Renderer* renderer;
	Level* m_plevel;
	Text2D* m_fpsCount;
	Time m_time;
	int m_fps = 0;

	vector <Tile*> m_vWalls;
	vector <Tile*> m_vFloor;
	Player* m_pPlayer;

	//screen properties
	const float m_screenWidth = 1920.0f;
	const float m_screenHeight = 1080.0f;
	const float m_cNearClip = 0.1f;
	const float m_cFarClip = 100.0f;
	const float m_cOrthographicSize = 100.0f;
};

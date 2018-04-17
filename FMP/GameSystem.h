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
#include "Monster.h"
#include "Projectile.h"
#include "Asset.h"

class GameSystem
{
public:
	GameSystem();
	~GameSystem();

	//play the main game loop
	int playGame(MSG msg, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	
	//set up all the objects in the game
	void SetupLevel();

	//Get the keyboard input
	void GetKeyboardInput();
	//Get the controller input
	void GetControllerInput();
	void GetMousePos();

	//player 1 controller
	CXBOXController* player1 = new CXBOXController(1);

	//Draws the whole level
	void DrawLevel(XMMATRIX view, XMMATRIX projection);
	void UpdateText();

private:
	//Level objects
	Renderer* renderer;
	Level* m_plevel;
	Text2D* m_fpsCount;
	Text2D* m_monsterLOS;
	Time m_time;
	Player* m_pPlayer;
	Monster* m_pMonster;
	Asset* m_viewCone;

	//vector objects
	vector <Tile*> m_tileMap;
	vector <Projectile*> m_vProjectiles;
	vector <Asset*> m_vDoors;

	//frames per second
	int m_fps = 0;
	//calculated every frame
	float m_deltaTime;

	//screen properties
	const float m_screenWidth = 1920.0f;
	const float m_screenHeight = 1080.0f;
	const float m_cNearClip = 0.05f;
	const float m_cFarClip = 100.0f;
	const float m_cOrthographicSize = 100.0f;

	//Used as a toggle for the mouse
	bool m_isMousePressed = false;

	int m_bulletNum = 0;

	float mouseX;
	float mouseY;
};

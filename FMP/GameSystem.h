#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>

#include <DirectXMath.h>
using namespace DirectX;

#include <vector>

#include "Renderer.h"
#include "text2D.h"
#include "Entity.h"
#include "Level.h"

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


private:
	Renderer* renderer;
	//Level* m_plevel;
};

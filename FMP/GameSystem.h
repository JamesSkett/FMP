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
#include "StateMachine.h"

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
	void GetMousePos();

	//player 1 controller
	CXBOXController* player1 = new CXBOXController(1);

	//Draws the whole level
	void DrawLevel(XMMATRIX view, XMMATRIX projection);
	void UpdateText();

	void SoundWaveWalk();
	void SoundWaveSprint();
	void SoundWaveDoorOpen();

private:
	//Level objects
	Renderer* renderer;
	Level* m_plevel;
	Time m_time;
	Player* m_pPlayer;
	Monster* m_pMonster;

	Asset* m_viewConeEnemy;
	Asset* m_viewConePlayer;
	Asset* m_soundWaveWalk;
	Asset* m_soundWaveSprint;
	Asset* m_soundWaveDoorOpen;

	StateMachine* m_stateMachine;

	Text2D* m_text_fpsCount;
	Text2D* m_text_monsterLOS;
	Text2D* m_text_currentState;
	
	//vector objects
	vector <Tile*> m_tileMap;
	vector <Projectile*> m_vProjectiles;

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

	//Level Script file path
	const char* LEVEL_DATA_FILE_PATH = "Scripts/Level_Data.txt";

	//View cone start properties
	const char* VIEW_CONE_FILE_PATH = "Assets/viewCone2.png";
	const float c_viewConeStartX = 0.0f;
	const float c_viewConeStartY = 0.0f;
	const float c_viewConeZPos = 3.0f;
	const float c_viewConeScale = 2.5f;
	const float c_viewConeRotation = 0.0f;

	//sound wave start properties
	const char* SOUND_WAVE_FILE_PATH = "Assets/soundWave.png";
	const float c_soundWaveZPos = 3.0f;
	const float c_soundWaveScale = 0.0f;
	const float c_soundWaveRotation = 0.0f;

	//Text font file path
	const char* FONT_FILE_PATH = "Assets/myFont.png";

	//fps text properties
	const float c_fpsTextXPos = -0.99f;
	const float c_fpsTextYPos = -0.95f;
	const float c_fpsTextScale =  0.03f;

	//line of sight text properties
	const float c_LOSTextPosX  = -0.99f;
	const float c_LOSTextPosY  =  0.99f;
	const float c_LOSTextScale =  0.033f;

	//current state text properties
	const float c_currentStateTextXPos = 0.0f;
	const float c_currentStateTextYPos = 0.99f;
	const float c_currentStateTextScale = 0.033f;

	//For the sound waves
	const float c_soundWalkScale = 0.8f;
	const float c_soundWalkSpeed = 40.0f;
	const float c_soundSprintScale = 2.0f;
	const float c_soundSprintSpeed = 75.0f;
	const float c_soundZeroScale = 0.0f;
	const float c_soundDoorScale = 3.0f;
	const float c_soundDoorSpeed = 80.0f;

	//Used as a toggle for the mouse and keyboard
	bool m_isMousePressed = false;
	bool m_isKeyPressed = false;
	bool m_lerpDown = false;
	bool m_lerpDownDoor = false;
	bool m_doorSound = false;

	int m_bulletNum = 0;

	float mouseX;
	float mouseY;

	
};

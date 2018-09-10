#include "GameSystem.h"
#include "Pathfinding.h"
#include "Renderer.h"
#include "text2D.h"
#include "Level.h"
#include "Player.h"
#include "Time.h"
#include "Monster.h"
#include "Projectile.h"
#include "Tile.h"
#include "StateMachine.h"
#include "CXBOXController.h"
#include "Camera.h"
#include "Mesh.h"
#include "Scene_Node.h"

//set up the renderer and main menu
GameSystem::GameSystem()
{
	renderer = new Renderer;
	m_plevel = new Level;
	player1 = new CXBOXController(1);
	m_pPlayer = nullptr;
	m_pMonster = nullptr;
	m_text_fpsCount = nullptr;
	m_text_monsterLOS = nullptr;
	m_text_currentState = nullptr;
	m_viewConeEnemy = nullptr;
	m_viewConePlayer = nullptr;
	m_soundWaveWalk = nullptr;
	m_soundWaveSprint = nullptr;
	m_soundWaveDoorOpen = nullptr;
	m_stateMachine = nullptr;
}

//clean up before exiting
GameSystem::~GameSystem()
{
	if (renderer)
	{
		delete renderer;
		renderer = nullptr;
	}

	if (m_plevel)
	{
		delete m_plevel;
		m_plevel = nullptr;
	}

	if (m_pMonster)
	{
		delete m_pMonster;
		m_pMonster = nullptr;
	}

	if (m_pPlayer)
	{
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}

	if (m_text_fpsCount)
	{
		delete m_text_fpsCount;
		m_text_fpsCount = nullptr;
	}

	if (m_text_monsterLOS)
	{
		delete m_text_monsterLOS;
		m_text_monsterLOS = nullptr;
	}

	if (m_text_currentState)
	{
		delete m_text_currentState;
		m_text_currentState = nullptr;
	}

	for (unsigned int i = 0; i < m_tileMap.size(); i++)
	{
		delete m_tileMap[i];
		m_tileMap[i] = nullptr;
	}

	m_tileMap.clear();

	if (m_stateMachine)
	{
		delete m_stateMachine;
		m_stateMachine = nullptr;
	}
}

//set up the game and run the main game loop
int GameSystem::playGame(MSG msg, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (FAILED(renderer->InitialiseWindow(hInstance, nCmdShow)))
	{
		DXTRACE_MSG("Failed to create Window");
		return 0;
	}

	if (FAILED(renderer->InitialiseInput()))
	{
		DXTRACE_MSG("Failed to Set up input");
		return 0;
	}

	if (FAILED(renderer->InitialiseD3D()))
	{
		DXTRACE_MSG("Failed to create Device");
		return 0;
	}

	if (FAILED(renderer->InitialiseGraphics()))
	{
		DXTRACE_MSG("Failed to Initialise Graphics");
		return 0;
	}

	//set up the main game when menu is done
	//SetupLevel();

	//load the level data
	m_plevel->LoadLevelData(LEVEL_DATA_FILE_PATH);
	//use the data to create the tiles
	m_plevel->SetUpLevelLayout(m_tileMap, m_pPlayer, m_pMonster);


	m_text_fpsCount = new Text2D(FONT_FILE_PATH, Renderer::pD3DDevice, Renderer::pImmediateContext);

	m_pMesh1 = new Mesh(Renderer::pD3DDevice, Renderer::pImmediateContext);
	m_pMesh1->LoadObjModel("Assets/floor.obj");
	m_pMesh1->AddTexture("Assets/floor.jpg");

	m_pMesh2 = new Mesh(Renderer::pD3DDevice, Renderer::pImmediateContext);
	m_pMesh2->LoadObjModel("Assets/cube.obj");
	m_pMesh2->AddTexture("Assets/wall.jpg");

	m_pPlayerMesh = new Mesh(Renderer::pD3DDevice, Renderer::pImmediateContext);
	m_pPlayerMesh->LoadObjModel("Assets/cube.obj");
	m_pPlayerMesh->AddTexture("Assets/grey_texture.bmp");

	m_pRootNode = new Scene_Node();

	for (unsigned int i = 0; i < m_tileMap.size(); i++)
	{
		if (m_tileMap[i]->GetIndex() == 2)
		{
			m_tileMap[i]->SetModel(m_pMesh2);
		}
		else
		{
			m_tileMap[i]->SetModel(m_pMesh1);
		}

		m_pRootNode->AddChildNode(m_tileMap[i]);
	}

	m_pPlayer->SetModel(m_pPlayerMesh);
	m_pRootNode->AddChildNode(m_pPlayer);

	Renderer::camera->SetX(m_pPlayer->GetXPos());
	Renderer::camera->SetY(m_pPlayer->GetYPos() + 0.1f);
	Renderer::camera->SetZ(m_pPlayer->GetZPos());
	Renderer::camera->SetPlayer(m_pPlayer);

	float currentTime = 0;
	float previousTime = 0;

	//Main game loop
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			previousTime = currentTime;
			currentTime = clock() / 10000.0f;
			//get the deltatime
			m_deltaTime = currentTime - previousTime;



			//Get the controller and keyboard input
			//GetControllerInput();
			renderer->ReadInputState();
			GetKeyboardInput();
			GetControllerInput();
			GetMousePos();

			// Clear the back buffer - sets a background colour
			float rgba_clear_colour[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
			Renderer::pImmediateContext->ClearRenderTargetView(Renderer::pBackBufferRTView, rgba_clear_colour);

			Renderer::pImmediateContext->ClearDepthStencilView(Renderer::m_pzBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			XMMATRIX identity, view, projection;

			float w = m_screenWidth / m_cOrthographicSize;
			float h = m_screenHeight / m_cOrthographicSize;

			//projection = XMMatrixOrthographicLH(w, h, m_cNearClip, m_cFarClip);

			identity = XMMatrixIdentity();
			projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(40.0f), m_screenWidth / m_screenHeight, 0.1f, 500.0f);
			view = Renderer::camera->GetViewMatrix(m_tileMap);

			m_pPlayer->SetXPos(Renderer::camera->GetX());
			m_pPlayer->SetYPos(Renderer::camera->GetY());
			m_pPlayer->SetZPos(Renderer::camera->GetZ());

			m_pRootNode->Execute(&identity, &view, &projection);

			//m_pPlayer->Update(XMFLOAT2(m_pMonster->GetXPos(), m_pMonster->GetYPos()), m_tileMap, m_deltaTime);
			//m_pMonster->Update(m_pPlayer, m_deltaTime);

			//if (m_pPlayer->GetWalkedThroughDoor())
			//{
			//	m_doorSound = true;
			//	m_soundWaveDoorOpen->SetPos(m_pPlayer->GetDoorPos().x, m_pPlayer->GetDoorPos().y);
			//}

			//SoundWaveDoorOpen();

			//m_stateMachine->RunStateMachine(m_pPlayer, m_pMonster, m_deltaTime);

			//m_soundWaveWalk->SetPos(m_pPlayer->GetXPos(), m_pPlayer->GetYPos());
			//m_soundWaveSprint->SetPos(m_pPlayer->GetXPos(), m_pPlayer->GetYPos());


			////if the monster is chasing the player
			//if (m_stateMachine->GetCurrentState() == "Chase")
			//{
			//	//if the monster catches the player reset the positions and the state
			//	//state probabilities DONT reset
			//	if (m_pMonster->CollisionCheck(m_pPlayer))
			//	{
			//		m_pPlayer->Reset();
			//		m_pMonster->Reset();
			//		m_stateMachine->SetCurrentState(RANDOM_WANDER);
			//	}
			//}

			//DrawLevel(view, projection);

			UpdateText();


			renderer->RenderFrame();

		}
	}

	//release all object before exiting
	renderer->ShutdownD3D();

	return 0;
}

//set up the main game level
void GameSystem::SetupLevel()
{
	//load the level data
	m_plevel->LoadLevelData(LEVEL_DATA_FILE_PATH);
	//use the data to create the tiles
	m_plevel->SetUpLevelLayout(m_tileMap, m_pPlayer, m_pMonster);
	//pass the tile map to the pathfinder class in Monster
	m_pMonster->SetPathfinder(m_tileMap);

	//create the texts
	m_text_fpsCount         = new Text2D(FONT_FILE_PATH, Renderer::pD3DDevice, Renderer::pImmediateContext);
	m_text_monsterLOS		= new Text2D(FONT_FILE_PATH, Renderer::pD3DDevice, Renderer::pImmediateContext);
	m_text_currentState		= new Text2D(FONT_FILE_PATH, Renderer::pD3DDevice, Renderer::pImmediateContext);

	//create the state machine
	m_stateMachine			= new StateMachine();

	//set the viewcones in the player and monster classe
	m_pPlayer->SetViewCone(m_viewConePlayer);
	m_pMonster->SetViewCone(m_viewConeEnemy);
}

bool sprinting = false;
//Get the keyboard input
void GameSystem::GetKeyboardInput()
{
	if (renderer->IsKeyPressed(DIK_W))
	{
		Renderer::camera->Forward(10.5f, m_deltaTime);
	}

	if (renderer->IsKeyPressed(DIK_S))
	{
		Renderer::camera->Forward(-10.5f, m_deltaTime);
	}

	if (renderer->IsKeyPressed(DIK_A))
	{
		Renderer::camera->Strafe(-10.5f, m_deltaTime);
	}

	if (renderer->IsKeyPressed(DIK_D))
	{
		Renderer::camera->Strafe(10.5f, m_deltaTime);
	}

	if (renderer->IsKeyPressed(DIK_COMMA))
	{
		Renderer::camera->Up(-0.5f);
	}

	if (renderer->IsKeyPressed(DIK_PERIOD))
	{
		Renderer::camera->Up(0.5f);
	}


	if ((renderer->mouseCurrState.lX != renderer->mouseLastState.lX) || (renderer->mouseCurrState.lY != renderer->mouseLastState.lY))
	{
		Renderer::camera->UpdateYaw(renderer->mouseLastState.lX * 0.001f);

		Renderer::camera->UpdatePitch(renderer->mouseCurrState.lY * 0.001f);

		renderer->mouseLastState = renderer->mouseCurrState;
	}
}

void GameSystem::GetControllerInput()
{
	//gets the x and y values for the controller thumbsticks
	float leftSticValY = player1->GetState().Gamepad.sThumbLY / 100.0f;
	float leftSticValX = player1->GetState().Gamepad.sThumbLX / 100.0f;

	float rightSticValY = player1->GetState().Gamepad.sThumbRY / 1000.0f;
	float rightSticValX = player1->GetState().Gamepad.sThumbRX / 1000.0f;

	if (player1->IsConnected())
	{
		if ((leftSticValY > 100) || (leftSticValY < -100))
		{
			Renderer::camera->Forward(leftSticValY / 10, m_deltaTime);
		}

		if ((leftSticValX > 100) || (leftSticValX < -100))
		{
			Renderer::camera->Strafe(leftSticValX / 10, m_deltaTime);
		}

		if ((rightSticValY > 10) || (rightSticValY < -10))
		{
			Renderer::camera->UpdatePitch(-rightSticValY * 0.001f);
		}

		if ((rightSticValX > 10) || (rightSticValX < -10))
		{
			Renderer::camera->UpdateYaw(rightSticValX * 0.001f);
		}
	}
}

void GameSystem::GetMousePos()
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);

	mouseX = (float)cursorPos.x;
	mouseY = (float)cursorPos.y;

	mouseX = ((mouseX / m_screenWidth) * 2.0f) - 1.0f;
	mouseY = ((mouseY / m_screenHeight) * 2.0f) - 1.0f;
}


void GameSystem::DrawLevel(XMMATRIX view, XMMATRIX projection)
{
	for (unsigned int i = 0; i < m_tileMap.size(); i++)
	{
		m_tileMap[i]->Draw(view, projection);
	}
}

void GameSystem::UpdateText()
{
	//Gets the fps and converts it to text
	m_fps = m_time.GetFPS();
	string fps = "FPS:";
	fps = fps + to_string(m_fps);
	m_text_fpsCount->AddText(fps, c_fpsTextXPos, c_fpsTextYPos, c_fpsTextScale);

	/*string inSight;
	if (m_pMonster->GetPlayerInSight())
	{
		inSight = "Player in Sight: True";
	}
	else
	{
		inSight = "Player in Sight: False";
	}

	m_text_monsterLOS->AddText(inSight, c_LOSTextPosX, c_LOSTextPosY, c_LOSTextScale);

	string currentState = "Current State: ";
	currentState = currentState + m_stateMachine->GetCurrentState();
	m_text_currentState->AddText(currentState, c_currentStateTextXPos, c_currentStateTextYPos, c_currentStateTextScale);

	if (Renderer::s_FogOfWar == 1)
	{
		m_text_currentState->m_draw = false;
		m_text_monsterLOS->m_draw = false;
	}
	else
	{
		m_text_currentState->m_draw = true;
		m_text_monsterLOS->m_draw = true;
	}*/

	Renderer::pImmediateContext->OMSetBlendState(Renderer::pAlphaBlendEnable, 0, 0xffffffff);

	m_text_fpsCount->RenderText();
	/*m_text_monsterLOS->RenderText();
	m_text_currentState->RenderText();*/

	Renderer::pImmediateContext->OMSetBlendState(Renderer::pAlphaBlendDisable, 0, 0xffffffff);
}



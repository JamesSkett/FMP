#include "GameSystem.h"
#include "Pathfinding.h"
#include <thread>

//set up the renderer and main menu
GameSystem::GameSystem()
{
	renderer = new Renderer;
	m_plevel = new Level;
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

	if (m_soundWaveWalk)
	{
		delete m_soundWaveWalk;
		m_soundWaveWalk = nullptr;
	}

	if (m_soundWaveSprint)
	{
		delete m_soundWaveSprint;
		m_soundWaveSprint = nullptr;
	}

	if (m_soundWaveDoorOpen)
	{
		delete m_soundWaveDoorOpen;
		m_soundWaveDoorOpen = nullptr;
	}

	for (unsigned int i = 0; i < m_tileMap.size(); i++)
	{
		delete m_tileMap[i];
		m_tileMap[i] = nullptr;
	}

	m_tileMap.clear();

	for (unsigned int i = 0; i < m_vProjectiles.size(); i++)
	{
		delete m_vProjectiles[i];
		m_vProjectiles[i] = nullptr;
	}

	m_vProjectiles.clear();

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

	//run the main menu
	//m_mainMenu->StartMenu(Renderer::m_pD3DDevice, Renderer::m_pImmediateContext, renderer);

	//set up the main game when menu is done
	SetupLevel();

	/*Pathfinding* pathfinder = new Pathfinding(m_tileMap);


	vector <XMFLOAT2> waypoints;
	int waypontNum = 0;*/

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
			GetMousePos();

			// Clear the back buffer - choose a colour you like
			float rgba_clear_colour[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
			Renderer::pImmediateContext->ClearRenderTargetView(Renderer::pBackBufferRTView, rgba_clear_colour);

			XMMATRIX view, projection;

			float w = m_screenWidth / m_cOrthographicSize;
			float h = m_screenHeight / m_cOrthographicSize;

			projection = XMMatrixOrthographicLH(w, h, m_cNearClip, m_cFarClip);
			view = XMMatrixIdentity();

			m_pPlayer->Update(XMFLOAT2(m_pMonster->GetXPos(), m_pMonster->GetYPos()), m_tileMap, m_deltaTime);
			m_pMonster->Update(m_pPlayer, m_deltaTime);

			if (m_pPlayer->GetWalkedThroughDoor())
			{
				m_doorSound = true;
				m_soundWaveDoorOpen->SetPos(m_pPlayer->GetDoorPos().x, m_pPlayer->GetDoorPos().y);
			}

			SoundWaveDoorOpen();

			m_stateMachine->RunStateMachine(m_pPlayer, m_pMonster, m_deltaTime);

			m_soundWaveWalk->SetPos(m_pPlayer->GetXPos(), m_pPlayer->GetYPos());
			m_soundWaveSprint->SetPos(m_pPlayer->GetXPos(), m_pPlayer->GetYPos());



			for (unsigned int i = 0; i < m_vProjectiles.size(); i++)
			{
				if (m_vProjectiles[i]->GetIsFired())
				{
					m_vProjectiles[i]->Update(m_deltaTime);

					if (m_vProjectiles[i]->CollisionCheck(m_tileMap))
					{
						m_vProjectiles[i]->SetXPos(10.0f);
						m_vProjectiles[i]->SetYPos(10.0f);
						m_vProjectiles[i]->SetIsFired(false);
					}
					else if (m_vProjectiles[i]->CollisionCheck(m_pMonster))
					{
						m_vProjectiles[i]->SetXPos(10.0f);
						m_vProjectiles[i]->SetYPos(10.0f);
						m_vProjectiles[i]->SetIsFired(false);
					}
				}
			}

			DrawLevel(view, projection);

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
	m_plevel->LoadLevelData("scripts/Level_Data.txt");
	m_plevel->SetUpLevelLayout(m_tileMap, m_pPlayer, m_pMonster);
	m_plevel->LoadProjectiles(m_vProjectiles);
	m_pMonster->SetPathfinder(m_tileMap);

	m_viewConeEnemy = new Asset("Assets/viewCone2.png", 0, 0, 3, 2.0f, 0, 0, 0);
	m_viewConePlayer = new Asset("Assets/viewCone2.png", 0, 0, 3, 2.0f, 0, 0, 0);
	m_soundWaveWalk = new Asset(SOUND_WAVE_FILE_PATH, m_pPlayer->GetXPos(), m_pPlayer->GetYPos(), 3, 0.0f, 0, 0, 0);
	m_soundWaveSprint = new Asset(SOUND_WAVE_FILE_PATH, m_pPlayer->GetXPos(), m_pPlayer->GetYPos(), 3, 0.0f, 0, 0, 0);
	m_soundWaveDoorOpen = new Asset(SOUND_WAVE_FILE_PATH, m_pPlayer->GetXPos(), m_pPlayer->GetYPos(), 3, 0.0f, 0, 0, 0);
	
	m_soundWaveWalk->SetColour(Renderer::colour.Aqua);
	m_soundWaveSprint->SetColour(Renderer::colour.Aqua);
	m_soundWaveDoorOpen->SetColour(Renderer::colour.Aqua);
	
	m_pPlayer->SetViewCone(m_viewConePlayer);
	m_pMonster->SetViewCone(m_viewConeEnemy);

	m_text_fpsCount = new Text2D("Assets/myFont.png", Renderer::pD3DDevice, Renderer::pImmediateContext);
	m_text_monsterLOS = new Text2D("Assets/myFont.png", Renderer::pD3DDevice, Renderer::pImmediateContext);
	m_text_currentState = new Text2D("Assets/myFont.png", Renderer::pD3DDevice, Renderer::pImmediateContext);

	m_stateMachine = new StateMachine();
}

bool sprinting = false;
//Get the keyboard input
void GameSystem::GetKeyboardInput()
{
	if (renderer->IsKeyPressed(DIK_LSHIFT))
	{
		m_pPlayer->SprintOn();
		sprinting = true;
	}

	if (!renderer->IsKeyPressed(DIK_LSHIFT))
	{
		m_pPlayer->SprintOff();
		sprinting = false;
	}

	if (renderer->IsKeyPressed(DIK_W))
	{
		m_pPlayer->UpdateYPos(m_tileMap, true, m_deltaTime);

		if (!sprinting)
		{
			SoundWaveWalk();
		}
		else
		{
			SoundWaveSprint();
		}

	}

	else if (renderer->IsKeyPressed(DIK_S))
	{
		m_pPlayer->UpdateYPos(m_tileMap, false, m_deltaTime);

		if (!sprinting)
		{
			SoundWaveWalk();
		}
		else
		{
			SoundWaveSprint();
		}
	}

	else if (renderer->IsKeyPressed(DIK_D))
	{
		m_pPlayer->UpdateXPos(m_tileMap, true, m_deltaTime);

		if (!sprinting)
		{
			SoundWaveWalk();
		}
		else
		{
			SoundWaveSprint();
		}
	}

	else if (renderer->IsKeyPressed(DIK_A))
	{
		m_pPlayer->UpdateXPos(m_tileMap, false, m_deltaTime);

		if (!sprinting)
		{
			SoundWaveWalk();
		}
		else
		{
			SoundWaveSprint();
		}
	}
	else
	{
		m_soundWaveWalk->SetScale(0.0f);
		m_soundWaveSprint->SetScale(0.0f);
	}


	if (renderer->mouseCurrState.rgbButtons[0])
	{
		if (!m_isMousePressed)
		{
			m_vProjectiles[m_bulletNum]->SetIsFired(true);

			m_vProjectiles[m_bulletNum]->SetIsFired(true);

			m_vProjectiles[m_bulletNum]->SetXPos(m_pPlayer->GetXPos());
			m_vProjectiles[m_bulletNum]->SetYPos(m_pPlayer->GetYPos());

			m_vProjectiles[m_bulletNum]->SetDirection(m_pPlayer->GetDirectionX(), m_pPlayer->GetDirectionY());

			m_vProjectiles[m_bulletNum]->SetRotation(m_pPlayer->GetRotation());

			m_bulletNum++;

			if (m_bulletNum == 50)
			{
				m_bulletNum = 0;
			}

			m_isMousePressed = true;
		}
	}

	if (!renderer->mouseCurrState.rgbButtons[0])
	{
		m_isMousePressed = false;
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

	for (unsigned int i = 0; i < m_vProjectiles.size(); i++)
	{
		if (m_vProjectiles[i]->GetIsFired())
		{
			m_vProjectiles[i]->Draw(view, projection);
		}
	}

	Renderer::pImmediateContext->OMSetBlendState(Renderer::pAlphaBlendEnable, 0, 0xffffffff);
	m_soundWaveWalk->Draw(view, projection);
	m_soundWaveSprint->Draw(view, projection);
	m_soundWaveDoorOpen->Draw(view, projection);
	Renderer::pImmediateContext->OMSetBlendState(Renderer::pAlphaBlendEnable, 0, 0xffffffff);


	m_pPlayer->Draw(view, projection);
	m_pMonster->Draw(view, projection);
}

void GameSystem::UpdateText()
{
	//Gets the fps and converts it to text
	m_fps = m_time.GetFPS();
	string fps = "FPS:";
	fps = fps + to_string(m_fps);
	m_text_fpsCount->AddText(fps, -0.99f, -0.95f, 0.03f);

	string inSight;
	if (m_pMonster->GetPlayerInSight())
	{
		inSight = "Player in Sight: True";
	}
	else
	{
		inSight = "Player in Sight: False";

	}

	m_text_monsterLOS->AddText(inSight, -0.99f, 0.99f, 0.033f);

	string currentState = "Current State: ";
	currentState = currentState + m_stateMachine->GetCurrentState();
	m_text_currentState->AddText(currentState, 0.0f, 0.99f, 0.033f);

	Renderer::pImmediateContext->OMSetBlendState(Renderer::pAlphaBlendEnable, 0, 0xffffffff);

	m_text_fpsCount->RenderText();
	m_text_monsterLOS->RenderText();
	m_text_currentState->RenderText();

	Renderer::pImmediateContext->OMSetBlendState(Renderer::pAlphaBlendDisable, 0, 0xffffffff);
}

void GameSystem::SoundWaveWalk()
{
	m_soundWaveSprint->SetScale(0.0f);

	if (m_soundWaveWalk->GetScale() < m_soundWalkScale && !m_lerpDown)
	{
		m_soundWaveWalk->SetScale(m_soundWaveWalk->GetScale() + m_soundWalkSpeed * m_deltaTime);

		if (m_soundWaveWalk->GetScale() > m_soundWalkScale)
		{
			m_lerpDown = true;
		}
	}
	else if (m_lerpDown)
	{
		m_soundWaveWalk->SetScale(m_soundWaveWalk->GetScale() - m_soundWalkSpeed * m_deltaTime);
		if (m_soundWaveWalk->GetScale() < 0.001f)
		{
			m_lerpDown = false;
		}
	}
}

void GameSystem::SoundWaveSprint()
{
	m_soundWaveWalk->SetScale(0.0f);

	if (m_soundWaveSprint->GetScale() < m_soundSprintScale && !m_lerpDown)
	{
		m_soundWaveSprint->SetScale(m_soundWaveSprint->GetScale() + m_soundSprintSpeed * m_deltaTime);

		if (m_soundWaveSprint->GetScale() > m_soundSprintScale)
		{
			m_lerpDown = true;
		}
	}
	else if (m_lerpDown)
	{
		m_soundWaveSprint->SetScale(m_soundWaveSprint->GetScale() - m_soundSprintSpeed * m_deltaTime);
		if (m_soundWaveSprint->GetScale() < 0.0f)
		{
			m_lerpDown = false;
		}
	}
}

void GameSystem::SoundWaveDoorOpen()
{
	if (m_doorSound)
	{
		if (m_soundWaveDoorOpen->GetScale() < m_soundDoorScale && !m_lerpDownDoor)
		{
			m_soundWaveDoorOpen->SetScale(m_soundWaveDoorOpen->GetScale() + m_soundDoorSpeed * m_deltaTime);

			if (m_soundWaveDoorOpen->GetScale() > m_soundDoorScale)
			{
				m_lerpDownDoor = true;
			}
		}
		else if (m_lerpDownDoor)
		{
			m_soundWaveDoorOpen->SetScale(m_soundWaveDoorOpen->GetScale() - m_soundDoorSpeed * m_deltaTime);
			if (m_soundWaveDoorOpen->GetScale() < 0.0f)
			{
				m_lerpDownDoor = false;
				m_doorSound = false;
				m_soundWaveDoorOpen->SetScale(0.0f);
			}
		}
	}
}


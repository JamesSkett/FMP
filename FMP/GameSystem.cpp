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
	m_fpsCount = nullptr;
	m_monsterLOS = nullptr;
	m_viewCone = nullptr;
}

//clean up before exiting
GameSystem::~GameSystem()
{
	if (renderer)
	{
		delete renderer;
		renderer = nullptr;
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

	if (m_fpsCount)
	{
		delete m_fpsCount;
		m_fpsCount = nullptr;
	}

	if (m_viewCone)
	{
		delete m_viewCone;
		m_viewCone = nullptr;
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

			m_pMonster->Update(XMFLOAT2(m_pPlayer->GetXPos(), m_pPlayer->GetYPos()), m_deltaTime);

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


			m_viewCone->SetPos(m_pMonster->GetXPos(), m_pMonster->GetYPos());
			m_viewCone->SetRotation(m_pMonster->GetRotation());

			if (m_pMonster->GetIsSearching())
			{
				m_viewCone->SetColour(Renderer::colour.OrangeRed);
			}
			else if (!m_pMonster->GetIsSearching() && !m_pMonster->GetPlayerInSight())
			{
				m_viewCone->SetColour(Renderer::colour.Black);
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

	m_viewCone = new Asset("Assets/viewCone2.png", 0, 0, 3, 2.0f, 0, 0);


	m_fpsCount = new Text2D("Assets/myFont.png", Renderer::pD3DDevice, Renderer::pImmediateContext);
	m_monsterLOS = new Text2D("Assets/myFont.png", Renderer::pD3DDevice, Renderer::pImmediateContext);
}

bool sprinting = false;
//Get the keyboard input
void GameSystem::GetKeyboardInput()
{
	if (renderer->IsKeyPressed(DIK_LSHIFT))
	{
		if (!sprinting)
		{
			m_pPlayer->SprintOn();
			sprinting = true;
		}
	}

	if (!renderer->IsKeyPressed(DIK_LSHIFT))
	{
		m_pPlayer->SprintOff();
		sprinting = false;
	}

	if (renderer->IsKeyPressed(DIK_W))
	{
		m_pPlayer->UpdateYPos(m_tileMap, true, m_deltaTime);
	}

	if (renderer->IsKeyPressed(DIK_S))
	{
		m_pPlayer->UpdateYPos(m_tileMap, false, m_deltaTime);
	}

	if (renderer->IsKeyPressed(DIK_D))
	{
		m_pPlayer->UpdateXPos(m_tileMap, true, m_deltaTime);
	}

	if (renderer->IsKeyPressed(DIK_A))
	{
		m_pPlayer->UpdateXPos(m_tileMap, false, m_deltaTime);
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

void GameSystem::GetControllerInput()
{
	//gets the x and y values for the controller thumbsticks
	float leftSticValY = player1->GetState().Gamepad.sThumbLY / 10000.0f;
	float leftSticValX = player1->GetState().Gamepad.sThumbLX / 10000.0f;

	float rightSticValY = player1->GetState().Gamepad.sThumbRY / 10000.0f;
	float rightSticValX = player1->GetState().Gamepad.sThumbRX / 10000.0f;


	if (player1->IsConnected())
	{
		if ((leftSticValY > 10) || (leftSticValY))
		{

		}

		if ((leftSticValX > 10) || (leftSticValX < -10))
		{

		}

		if ((rightSticValY > 10) || (rightSticValY < -10))
		{

		}

		if ((rightSticValX > 10) || (rightSticValX < -10))
		{

		}

		if (player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
		}

		if (player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
		}
	}
	else
	{
		printf("\n\tERROR! PLAYER 1 - XBOX 360 Controller Not Found!\n");
		//std::cout << "Press Any Key To Exit.";
		//std::cin.get();

	}
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
	m_viewCone->Draw(view, projection);
	Renderer::pImmediateContext->OMSetBlendState(Renderer::pAlphaBlendDisable, 0, 0xffffffff);


	m_pPlayer->Draw(view, projection);
	m_pMonster->Draw(view, projection);
}

void GameSystem::UpdateText()
{
	//Gets the fps and converts it to text
	m_fps = m_time.GetFPS();
	string fps = "FPS:";
	fps = fps + to_string(m_fps);
	m_fpsCount->AddText(fps, -0.99f, 0.99f, 0.02f);

	string inSight;
	if (m_pMonster->GetPlayerInSight())
	{
		inSight = "Player in Sight: True";
		m_viewCone->SetColour(Renderer::colour.Green);
	}
	else
	{
		inSight = "Player in Sight: False";
		m_viewCone->SetColour(Renderer::colour.Black);

	}

	m_monsterLOS->AddText(inSight, 0.5f, 0.99f, 0.02f);

	Renderer::pImmediateContext->OMSetBlendState(Renderer::pAlphaBlendEnable, 0, 0xffffffff);

	m_fpsCount->RenderText();
	m_monsterLOS->RenderText();

	Renderer::pImmediateContext->OMSetBlendState(Renderer::pAlphaBlendDisable, 0, 0xffffffff);
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

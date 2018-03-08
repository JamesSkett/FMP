#include "GameSystem.h"

#include <thread>


//set up the renderer and main menu
GameSystem::GameSystem()
{
	renderer = new Renderer;
	m_plevel = new Level;
}

//clean up before exiting
GameSystem::~GameSystem()
{
	if (renderer)
	{
		delete renderer;
		renderer = nullptr;
	}

	if (m_pPlayer)
	{
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}

	for (unsigned int i = 0; i < m_vFloor.size(); i++)
	{
		delete m_vFloor[i];
		m_vFloor[i] = nullptr;
	}

	m_vFloor.clear();

	for (unsigned int i = 0; i < m_vWalls.size(); i++)
	{
		delete m_vWalls[i];
		m_vWalls[i] = nullptr;
	}

	m_vWalls.clear();

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
	//SetupLevel();

	m_plevel->LoadLevelData("scripts/Level_Data.txt");
	m_plevel->SetUpLevelLayout(m_vWalls, m_vFloor, m_pPlayer);


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
			//get the deltatime
			float deltaTime = Renderer::time.GetDeltaTime();

			// Clear the back buffer - choose a colour you like
			float rgba_clear_colour[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
			Renderer::pImmediateContext->ClearRenderTargetView(Renderer::pBackBufferRTView, rgba_clear_colour);

			XMMATRIX view, projection;

			float w = m_screenWidth / m_cOrthographicSize;
			float h = m_screenHeight / m_cOrthographicSize;

			projection = XMMatrixOrthographicLH(w, h, m_cNearClip, m_cFarClip);
			view = XMMatrixIdentity();

			//Get the controller and keyboard input
			//GetControllerInput();
			renderer->ReadInputState();
			GetKeyboardInput();
			m_pPlayer->Update();
			
			for (unsigned int i = 0; i < m_vWalls.size(); i++)
			{
				m_pPlayer->CollisionCheck(m_vWalls[1]);
			}


			DrawLevel(view, projection);

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
	
}

//Get the keyboard input
void GameSystem::GetKeyboardInput()
{
	if (renderer->IsKeyPressed(DIK_1))
	{
		
	}

	if (renderer->IsKeyPressed(DIK_2))
	{
		
	}

	if (renderer->IsKeyPressed(DIK_W))
	{
		m_pPlayer->UpdateYPos(0.1f);
	}

	if (renderer->IsKeyPressed(DIK_S))
	{
		m_pPlayer->UpdateYPos(-0.1f);
	}

	if (renderer->IsKeyPressed(DIK_D))
	{
		m_pPlayer->UpdateXPos(0.1f);
	}

	if (renderer->IsKeyPressed(DIK_A))
	{
		m_pPlayer->UpdateXPos(-0.1f);
	}

	if (renderer->IsKeyPressed(DIK_UP))
	{
		
	}

	if (renderer->IsKeyPressed(DIK_DOWN))
	{
		
	}

	if (renderer->IsKeyPressed(DIK_RIGHT))
	{
		
	}

	if (renderer->IsKeyPressed(DIK_LEFT))
	{
		
	}

	if (renderer->IsKeyPressed(DIK_Q))
	{
	}
	if (renderer->IsKeyPressed(DIK_E))
	{
	}

	if (renderer->IsKeyPressed(DIK_N))
	{
	}

	if (renderer->IsKeyPressed(DIK_M))
	{
	}


	if (renderer->mouseCurrState.rgbButtons[0])
	{
		
	}

	if (!renderer->mouseCurrState.rgbButtons[0])
	{

	}

	if (renderer->IsKeyPressed(DIK_SPACE))
	{
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
	for (unsigned int i = 0; i < m_vFloor.size(); i++)
	{
		m_vFloor[i]->Draw(view, projection);
	}

	for (unsigned int i = 0; i < m_vWalls.size(); i++)
	{
		m_vWalls[i]->Draw(view, projection);
	}

	m_pPlayer->Draw(view, projection);
}

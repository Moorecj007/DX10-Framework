/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Application.cpp
* Description : The core of the Application is run here. The link between all components.
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "Application.h"

// Static Variables
Application* Application::s_pApp = 0;

// Global Variables
FILE* g_file;

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _cmdShow)
{
	// Seed the random based on the time
	srand((UINT)time(NULL));

	if (AllocConsole())
	{
		freopen_s(&g_file, "conout$", "w", stdout);
	}

	// Set the client width and height
	int clientWidth = 1000;
	int clientHeight = 1000;

	// Create the Application 
	Application* pApp = Application::GetInstance();
	
	if (pApp->CreateWindowApp(clientWidth, clientHeight, _hInstance) == true)
	{
		if (pApp->Initialise(clientWidth, clientHeight, _hInstance) == false)
		{
			pApp->DestroyInstance();
			return 0;
		}
		pApp->Execute();
	}
	
	// Delete all contents of the Application
	pApp->DestroyInstance();
	return 0;
}

LRESULT CALLBACK Application::WindowProc(HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{
	// Retrieve the Instance of the Application
	Application* pApp = Application::GetInstance();

	// Switch case dependent on the message sent
	switch (_uiMsg)
	{
		case WM_DESTROY:	// Fall Through
		case WM_CLOSE:
		{
			// Kill the application, this sends a WM_QUIT message.
			PostQuitMessage(0);	
		}
		break;
		case WM_KEYDOWN:
		{
			// Only accepts the input once per key press
			if (((_lParam >> 30) & 1) != 1)
			{
				pApp->SetKeyDown(_wParam, true);
			}
		}
		break;
		case WM_KEYUP:
		{
			pApp->SetKeyDown(_wParam, false);
		}
		break;
		case WM_LBUTTONDOWN:
		{
			pApp->SetMouseDown(true);
		}
		break;
		case WM_LBUTTONUP:
		{
			pApp->SetMouseDown(false);
			
		}
		break;
		default: break;
	} // End switch.

	// Process any messages left to process
	return (DefWindowProc(_hWnd, _uiMsg, _wParam, _lParam));
}

bool Application::CreateWindowApp(int _clientWidth, int _clientHeight, HINSTANCE _hInstance)
{
	WNDCLASSEX winClass;	// This will hold the class we create.

	// Fills in the window class structure.
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc = WindowProc;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hInstance = _hInstance;
	winClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)WHITE_BRUSH;
	winClass.lpszMenuName = NULL;
	winClass.lpszClassName = WINDOW_CLASS_NAME;
	winClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// Registers the window class
	VALIDATE(RegisterClassEx(&winClass));

	m_hWnd = CreateWindowEx(NULL,								// Extended style.
		WINDOW_CLASS_NAME,					// Class.
		L"DX10 Framework",					// Title.
		WS_VISIBLE | WS_CAPTION | WS_BORDER | WS_SYSMENU,// Windowed Mode
		0, 0,								// Initial x,y position for the top left corner of the window
		_clientWidth, _clientHeight,		// Initial width, height of the window
		NULL,								// Handle to parent.
		NULL,								// Handle to menu.
		_hInstance,							// Instance of this application.
		NULL);								// Extra creation parameters.

	// Check the window was created successfully.
	VALIDATE(m_hWnd);

	return true;	
}

int Application::Execute()
{
	MSG uiMsg;				// Generic message.

	// Enter main event loop.
	while (m_online)
	{
		while (PeekMessage(&uiMsg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&uiMsg);
			DispatchMessage(&uiMsg);
		}

		if (uiMsg.message == WM_QUIT)
		{
			break;
		}

		ExecuteOneFrame();
	}

	// Return to Windows
	return (static_cast<int>(uiMsg.wParam));
}

Application* Application::GetInstance()
{
	if (s_pApp == 0)
	{
		// If the application does not exist, create the application
		s_pApp = new Application();
	}
	// Always return the current instance of the application
	return s_pApp;
}

bool Application::Initialise(int _clientWidth, int _clientHeight, HINSTANCE _hInstance)
{
	m_state = APP_STATE_MAIN_MENU;	// TO DO JURAN - Change to title screen 
	m_isFullscreen = false;
	m_isSound = false;
	m_isRumble = false;

	// Save the client window sizes
	m_clientWidth = _clientWidth;
	m_clientHeight = _clientHeight;

	// Initialise boolean array for KeyDown to false
	m_pKeyDown = new bool[255];
	memset(m_pKeyDown, false, 255);

	// Create the Player One Pad controller
	m_pGamepadPlayerOne = new InputGamePad();
	VALIDATE(m_pGamepadPlayerOne->Initialise(1));

	VALIDATE(Initialise_DX10(_hInstance));

	/*m_pGame = new Game();
	VALIDATE(m_pGame->Initialise(m_pDX10_Renderer, 2));*/

	m_online = true;

	// Initialise all time keeping variables to default (zero) state
	m_pTimer = new Timer();
	m_pTimer->Reset();
	m_fps = 0;
	m_deltaTick = 0;
	m_fpsTimer = 0;
	
	return true;
}

bool Application::Initialise_DX10(HINSTANCE _hInstance)
{
	// Initialise the Renderer
	m_pDX10_Renderer = new DX10_Renderer();
	VALIDATE(m_pDX10_Renderer->Initialise(m_clientWidth, m_clientHeight, m_hWnd));

	// Initialise the Objects
	m_pCamera = new DX10_Camera_Debug();
	VALIDATE(m_pCamera->Initialise(m_pDX10_Renderer));
	m_pCamera->SetPostionVec({ 0, 0, -100.0f });
	m_pCamera->SetTargetVec({ 0, 0, 0 });
	m_pCamera->SetUpVec({ 0, 1, 0 });

	// Initialise Main Menu
	m_menus.push_back(new Menu());
	VALIDATE(m_menus.back()->Initialize(m_pDX10_Renderer, &m_hWnd, m_pGamepadPlayerOne));

	m_menus.back()->AddSprite("Tron/UI/tron_orbliteration.png", 1231, 424, 1, 4);
	m_menus.back()->AddSprite("Tron/Button/tron_button_start_fill.png", 481, 424, 1, 4); 
	m_menus.back()->AddSprite("Tron/Button/tron_button_instructions_fill.png", 1137, 424, 1, 4);
	m_menus.back()->AddSprite("Tron/Button/tron_button_options_fill.png", 669, 424, 1, 4);
	m_menus.back()->AddSprite("Tron/Button/tron_button_exit_fill.png", 387, 424, 1, 4);
	m_menus.back()->AddTitle(0, 0.75f);
	m_menus.back()->AddButton(MENU_STATE_START, 1, 0.5f);
	m_menus.back()->AddButton(MENU_STATE_INSTRUCTIONS, 2, 0.5f);
	m_menus.back()->AddButton(MENU_STATE_OPTIONS, 3, 0.5f);
	m_menus.back()->AddButton(MENU_STATE_EXIT, 4, 0.5f);

	// Initialise Match Menu
	m_menus.push_back(new Menu());
	VALIDATE(m_menus.back()->Initialize(m_pDX10_Renderer, &m_hWnd, m_pGamepadPlayerOne));

	m_menus.back()->AddSprite("Tron/UI/tron_numbers_fill.png", 1060, 424, 10, 4);
	m_menus.back()->AddButton(MENU_STATE_PLAYERS_2, 0, 0.5f, 2);
	m_menus.back()->AddButton(MENU_STATE_PLAYERS_3, 0, 0.5f, 3);
	m_menus.back()->AddButton(MENU_STATE_PLAYERS_4, 0, 0.5f, 4);

	// Initialise Options Menu
	m_menus.push_back(new Menu());
	VALIDATE(m_menus.back()->Initialize(m_pDX10_Renderer, &m_hWnd, m_pGamepadPlayerOne));

	m_menus.back()->AddSprite("Tron/Button/toggle_button.png", 95, 61, 1, 2);
	m_menus.back()->AddSprite("Tron/Button/tron_button_fullscreen_fill.png", 945, 424, 1, 4);
	m_menus.back()->AddSprite("Tron/Button/tron_button_sound_fill.png", 481, 424, 1, 4);
	m_menus.back()->AddSprite("Tron/Button/tron_button_rumble_fill.png", 575, 424, 1, 4);
	m_menus.back()->AddButton(MENU_STATE_FULL_SCREEN, 1, 0.25f);
	m_menus.back()->AddToggleButton(m_menus.back()->GetButton(0), 0, m_isFullscreen);
	m_menus.back()->AddButton(MENU_STATE_SOUND, 2, 0.25f);
	m_menus.back()->AddToggleButton(m_menus.back()->GetButton(1), 0, m_isSound);
	m_menus.back()->AddButton(MENU_STATE_RUMBLE, 3, 0.25f);
	m_menus.back()->AddToggleButton(m_menus.back()->GetButton(2), 0, m_isRumble);

	// Initialise Pause Menu
	m_menus.push_back(new Menu());
	VALIDATE(m_menus.back()->Initialize(m_pDX10_Renderer, &m_hWnd, m_pGamepadPlayerOne));

	m_menus.back()->AddSprite("Tron/Button/tron_button_resume_fill.png", 575, 424, 1, 4);
	m_menus.back()->AddSprite("Tron/Button/tron_button_instructions_fill.png", 1137, 424, 1, 4);
	m_menus.back()->AddSprite("Tron/Button/tron_button_options_fill.png", 669, 424, 1, 4);
	m_menus.back()->AddSprite("Tron/Button/tron_button_exit_fill.png", 387, 424, 1, 4);
	m_menus.back()->AddButton(MENU_STATE_RESUME, 0, 0.5f);
	m_menus.back()->AddButton(MENU_STATE_INSTRUCTIONS, 1, 0.5f);
	m_menus.back()->AddButton(MENU_STATE_OPTIONS, 2, 0.5f);
	m_menus.back()->AddButton(MENU_STATE_EXIT, 3, 0.5f);

	return true;
}

Application::~Application()
{
}

void Application::DestroyInstance()
{
	s_pApp->ShutDown();
	ReleasePtr(s_pApp);
}

void Application::ShutDown()
{
	// Delete and free memory for the Application variables
	ReleasePtr(m_pKeyDown);
	ReleasePtr(m_pTimer);

	// Delete and free the memory from the Renderer
	if (m_pDX10_Renderer != 0)
	{ 
		// DX10 pointers to release
		ReleasePtr(m_pCamera);
		
		// Gamepad input memory release
		ReleasePtr(m_pGamepadPlayerOne);

		// Menu memory release
		while (!m_menus.empty())
		{
			ReleasePtr(m_menus.back());
			m_menus.pop_back();
		}
		
		// Game play objects memory release
		ReleasePtr(m_pGame);

		// Release the renderers resources
		m_pDX10_Renderer->ShutDown();
		ReleasePtr(m_pDX10_Renderer);
	}	
}

void Application::ExecuteOneFrame()
{
	// Retrieve the Delta Tick of the last frame
	m_pTimer->Tick();
	float dt = m_pTimer->GetDeltaTime();
	m_deltaTick += dt;
	m_fpsTimer += dt;

	if (Process(dt) == false)
	{
		// A process failed to create something
		m_online = false;
		return;
	}

	// Limit to 60 FPS for Renderering
	if (m_deltaTick > (1.0 / 60.0f))
	{
		Render();
		m_deltaTick = 0;
		m_fps++;
	}	

	// Reset FPS counters
	if (m_fpsTimer >= 1.0f)
	{
		m_fpsTimer -= 1.0f;
		m_fps = 0;
	}
}

bool Application::Process(float _dt)
{
	VALIDATE(HandleInput());

	// Processes to run when using DX10 Renderer
	if (m_pDX10_Renderer != 0)
	{		
		m_pCamera->Process();

		ProcessShaders();

		switch (m_state)
		{
		case APP_STATE_TITLE:
			break;
		case APP_STATE_MAIN_MENU:
			m_menus[0]->Process(_dt);
			UpdateState(m_menus[0]->GetMenuState());
			break;
		case APP_STATE_MATCH_MENU:
			m_menus[1]->Process(_dt);
			UpdateState(m_menus[1]->GetMenuState());
			break;
		case APP_STATE_INSTRUCTIONS_MENU:
			break;
		case APP_STATE_OPTION_MENU:
			m_menus[2]->Process(_dt);
			UpdateState(m_menus[2]->GetMenuState());
			break;
		case APP_STATE_PAUSE_MENU:
			m_menus[3]->Process(_dt);
			UpdateState(m_menus[3]->GetMenuState());
			break;
		case APP_STATE_GAME:
			if (m_pGame->Process(_dt) == false)
			{
				// If the game has ended
				ReleasePtr(m_pGame);
				m_state = APP_STATE_MAIN_MENU;
				m_menus[0]->Reset();
 			}
			break;
		default:
			break;
		}
	}

	return true;
}

void Application::ProcessShaders()
{
	
}

void Application::Render()
{
	// Render calls when using the DX10 Renderer
	if (m_pDX10_Renderer != 0)
	{
		// Get the Renderer Ready to receive new data
		m_pDX10_Renderer->StartRender();

		switch (m_state)
		{
		case APP_STATE_TITLE:
			break;
		case APP_STATE_MAIN_MENU:
			m_menus[0]->Draw();
			break;
		case APP_STATE_MATCH_MENU:
			m_menus[1]->Draw();
			break;
		case APP_STATE_INSTRUCTIONS_MENU:
			break;
		case APP_STATE_OPTION_MENU:
			m_menus[2]->Draw();
			break;
		case APP_STATE_PAUSE_MENU:
			m_menus[3]->Draw();
			break;
		case APP_STATE_GAME:
			m_pGame->Render();
			break;
		default:
			break;
		}

		// Tell the Renderer the data input is over and present the outcome
		m_pDX10_Renderer->EndRender();	
	}
}

bool Application::HandleInput()
{
	if (m_pKeyDown[VK_ESCAPE])
	{
		ExitApp();
	}

	// Template Inputs
	if (m_pKeyDown[VK_F1])
	{
		m_pDX10_Renderer->ToggleFullscreen();

		SetKeyDown(VK_F1, false);
	}

	if (m_pKeyDown[VK_F2])
	{
		m_pDX10_Renderer->ToggleFillMode();

		SetKeyDown(VK_F2, false);
	}

	if ((m_pKeyDown[VK_NUMPAD4]) && !(m_pKeyDown[VK_NUMPAD6]))
	{
		m_pCamera->Strafe(-1 * m_deltaTick);
	}

	if ((m_pKeyDown[VK_NUMPAD6]) && !(m_pKeyDown[VK_NUMPAD4]))
	{
		m_pCamera->Strafe(m_deltaTick);
	}

	if ((m_pKeyDown[VK_NUMPAD8]) && !(m_pKeyDown[VK_NUMPAD2]))
	{
		m_pCamera->Move(m_deltaTick);
	}

	if ((m_pKeyDown[VK_NUMPAD2]) && !(m_pKeyDown[VK_NUMPAD8]))
	{
		m_pCamera->Move(-1 * m_deltaTick);
	}

	if ((m_pKeyDown[VK_NUMPAD1]) && !(m_pKeyDown[VK_NUMPAD0]))
	{
		m_pCamera->Fly(m_deltaTick);
	}

	if ((m_pKeyDown[VK_NUMPAD0]) && !(m_pKeyDown[VK_NUMPAD1]))
	{
		m_pCamera->Fly(-1 * m_deltaTick);
	}

	if ((m_pKeyDown[VK_UP]) && !(m_pKeyDown[VK_DOWN]))
	{
		m_pCamera->Pitch(-1 * m_deltaTick);
	}

	if ((m_pKeyDown[VK_DOWN]) && !(m_pKeyDown[VK_UP]))
	{
		m_pCamera->Pitch(m_deltaTick);
	}

	if ((m_pKeyDown[VK_LEFT]) && !(m_pKeyDown[VK_RIGHT]))
	{
		m_pCamera->Yaw(-1 * m_deltaTick);
	}

	if ((m_pKeyDown[VK_RIGHT]) && !(m_pKeyDown[VK_LEFT]))
	{
		m_pCamera->Yaw(m_deltaTick);
	}

	return true;
}

void Application::ExitApp()
{
	if (m_pDX10_Renderer != 0)
	{
		if (m_pDX10_Renderer->GetFullScreenState() == true)
		{
			// Toggle the screen mode to windowed before exiting application
			m_pDX10_Renderer->ToggleFullscreen();
		}
	}
	m_online = false;	// Changing this to false will cause the main application loop to end -> quitting the application
}

void Application::UpdateState(MENU_STATE _state)
{
	switch (_state)
	{
		// Main menu
	case MENU_STATE_START:
		m_state = APP_STATE_MATCH_MENU;
		m_menus[1]->Reset();
		break;
	case MENU_STATE_INSTRUCTIONS:
		//m_state = APP_STATE_INSTRUCTIONS_MENU;
		m_menus[0]->Reset();
		break;
	case MENU_STATE_OPTIONS:
		m_state = APP_STATE_OPTION_MENU;
		m_menus[2]->Reset();
		break;
	case MENU_STATE_EXIT:
		switch (m_state)
		{
		case APP_STATE_MAIN_MENU:
			ExitApp();
			break;
		case APP_STATE_MATCH_MENU:			// Fall through
		case APP_STATE_INSTRUCTIONS_MENU:	// Fall through
		case APP_STATE_OPTION_MENU:			// Fall through
			m_state = APP_STATE_MAIN_MENU;
			m_menus[0]->Reset();
			break;
		case APP_STATE_PAUSE_MENU:
			m_state = APP_STATE_GAME;
			break;
		}
		break;

		// Options menu
	case MENU_STATE_FULL_SCREEN:
	{
		m_state = APP_STATE_OPTION_MENU;
		m_isFullscreen = !m_isFullscreen;
		m_pDX10_Renderer->ToggleFullscreen();
		m_menus[2]->ToggleButton(0);
		m_menus[2]->Reset();
	}
		break;
	case MENU_STATE_SOUND:
	{
		m_state = APP_STATE_OPTION_MENU;
		m_isSound = !m_isSound;
		// Toggle sound
		m_menus[2]->ToggleButton(1);
		m_menus[2]->Reset();
	}
		break;
	case MENU_STATE_RUMBLE:
	{
		m_state = APP_STATE_OPTION_MENU;
		m_isRumble = !m_isRumble;
		// Toggle rumble
		m_menus[2]->ToggleButton(2);
		m_menus[2]->Reset();
	}
		break;

		// Match menu states
	case MENU_STATE_PLAYERS_2:
		m_pGame = new Game();
		if (m_pGame->Initialise(m_pDX10_Renderer, 2))
		{
			m_state = APP_STATE_GAME;
		}
		else
		{
			ReleasePtr(m_pGame);
			m_state = APP_STATE_MATCH_MENU;
			m_menus[1]->Reset();
		}
		break;
	case MENU_STATE_PLAYERS_3:
		m_pGame = new Game();
		if (m_pGame->Initialise(m_pDX10_Renderer, 3))
		{
			m_state = APP_STATE_GAME;
		}
		else
		{
			ReleasePtr(m_pGame);
			m_state = APP_STATE_MATCH_MENU;
			m_menus[1]->Reset();
		}
		break;
	case MENU_STATE_PLAYERS_4:
		m_pGame = new Game();
		if (m_pGame->Initialise(m_pDX10_Renderer, 4))
		{
			m_state = APP_STATE_GAME;
		}
		else
		{
			ReleasePtr(m_pGame);
			m_state = APP_STATE_MATCH_MENU;
			m_menus[1]->Reset();
		}
		break;

		// Pause menu states (reuses the main menu states)
	case MENU_STATE_RESUME:
		m_state = APP_STATE_GAME;
		break;

	default:
		
		break;
	}
}

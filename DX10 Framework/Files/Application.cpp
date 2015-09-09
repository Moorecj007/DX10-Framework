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
	#ifdef _DEBUG
		// Create a Console window in debug mode only for showing various information
		if (AllocConsole())
		{
			freopen_s(&g_file, "conout$", "w", stdout);
			HWND hWnd = GetConsoleWindow();
			ShowWindow(hWnd, 1);
		}
	#endif

	// Set the client width and height
	int clientWidth = 1000;
	int clientHeight = 1000;

	// Create the Application 
	Application* pApp = Application::GetInstance();
	
	// Add 300 to the width to create a side panel that only the application and backbuffer know about
	if (pApp->CreateWindowApp(clientWidth + 300, clientHeight, _hInstance) == true)
	{
		VALIDATE(pApp->Initialise(clientWidth, clientHeight, _hInstance));
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
			// Retrieves and sets the mouse position for when the left mouse button is first pressed
			v2float mousePos = { (float)GET_X_LPARAM(_lParam), (float)GET_Y_LPARAM(_lParam) };
			pApp->SetFirstMousePos(mousePos);
			pApp->SetMouseDown(true);
		}
		break;
		case WM_LBUTTONUP:
		{
			// Retrieves and sets the mouse position for when the left mouse button is released
			v2float mousePos = { (float)GET_X_LPARAM(_lParam), (float)GET_Y_LPARAM(_lParam) };
			pApp->SetSecondMousePos(mousePos);
			pApp->SetMouseDown(false);

			// Determines if any cuttable ropes were crossing the line created by the two mouse positions
			pApp->CutRope();
			
		}
		case WM_MOUSEMOVE:
		{
			// Continuously update the mouse second position for if the mouse button is held down
			v2float mousePos = { (float)GET_X_LPARAM(_lParam), (float)GET_Y_LPARAM(_lParam) };
			pApp->SetSecondMousePos(mousePos);
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

// Constructors / Destructors
Application::Application()
{

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
	// Determine which Rendererer to use
	bool usingDX10Renderer = false;
	bool usingGDIRenderer = true;

	// Default state so only one renderer can be active at any time
	if (usingDX10Renderer == usingGDIRenderer)
	{
		usingDX10Renderer = true;
		usingGDIRenderer = false;
	}

	// Save the client window sizes
	m_clientWidth = _clientWidth;
	m_clientHeight = _clientHeight;

	// Initialise boolean array for KeyDown to false
	m_pKeyDown = new bool[255];
	memset(m_pKeyDown, false, 255);

	// Create a Renderer for graphics
	if (usingDX10Renderer == true)
	{
		// Initialise the Renderer
		m_pDX10_Renderer = new DX10_Renderer();
		VALIDATE(m_pDX10_Renderer->Initialise(m_clientWidth, m_clientHeight, m_hWnd));
	
		// Initialise the Objects
		m_pCamera = new DX10_Camera_FirstPerson();
		m_pCamera->Initialise(m_pDX10_Renderer, _hInstance, m_hWnd);
	}
	
	if (usingGDIRenderer == true)
	{
		// Initialise the Renderer
		m_pGDIRenderer = new GDI_Renderer();
		VALIDATE(m_pGDIRenderer->Initialise(m_hWnd, _hInstance, m_clientWidth + 300, m_clientHeight));

		// Initialise the application to run the first level on startup
		m_pCurrentLevel = new Level_01(m_pGDIRenderer, m_clientWidth, m_clientHeight);
		m_pCurrentLevel->ContructLevel();
		m_levelSelection = LS_LEVEL01;
	}

	m_online = true;

	// Initialise all time keeping variables to default (zero) state
	m_pTimer = new Timer();
	m_pTimer->Reset();
	m_fps = 0;
	m_deltaTick = 0;
	m_fpsTimer = 0;
	
	return true;
}

Application::~Application()
{
}

void Application::DestroyInstance()
{
	s_pApp->ShutDown();
	delete s_pApp;
	s_pApp = 0;
}

void Application::ShutDown()
{
	// Delete and free memory for the Application variables
	ReleasePtr(m_pKeyDown);
	ReleasePtr(m_pTimer);

	// DX10 pointers to release
	ReleasePtr(m_pShader_LitTex);
	ReleasePtr(m_pCamera);
	ReleasePtr(m_pCubeMesh);
	ReleasePtr(m_pCube);

	// GDI pointers to release
	m_pCurrentLevel->DestroyLevel();
	ReleasePtr(m_pCurrentLevel);

	// Delete and free the memory from the Renderer
	if (m_pDX10_Renderer != 0)
	{
		m_pDX10_Renderer->ShutDown();
	}
	ReleasePtr(m_pDX10_Renderer);
	ReleasePtr(m_pGDIRenderer);
}

void Application::ExecuteOneFrame()
{
	// Retrieve the Delta Tick of the last frame
	m_pTimer->Tick();
	float dt = m_pTimer->GetDeltaTime();
	m_deltaTick += dt;
	m_fpsTimer += dt;

	// Limit to 60 FPS
	if (m_deltaTick > (1.0 / 60.0f))
	{
		Process(m_deltaTick);
		Draw();

		m_deltaTick -= (1.0f / 60.0f);
		m_fps++;
	}	

	// Reset FPS counters
	if (m_fpsTimer >= 1.0f)
	{
		// Print the FPS to the console if in Debug mode
		#ifdef _DEBUG
			printf("%d \n", m_fps);
		#endif
		
		m_fpsTimer -= 1.0f;
		m_fps = 0;
	}
}

void Application::Process(float _dt)
{
	HandleInput();

	// Processes to run when using DX10 Renderer
	if (m_pDX10_Renderer != 0)
	{
		m_pCamera->Process(_dt);
		m_pCube->Process(_dt);
		m_pShader_LitTex->SetUpPerFrame();
	}

	// Processes to run when using GDI Renderer
	if (m_pGDIRenderer != 0)
	{
		// If the process returns true the level has been completed and so changes the current level to the next leve;l
		if (m_pCurrentLevel->Process(_dt) == true)
		{
			switch (m_levelSelection)
			{
				case LS_LEVEL01:
				{
					m_pCurrentLevel->DestroyLevel();
					ReleasePtr(m_pCurrentLevel);
					m_pCurrentLevel = new Level_02(m_pGDIRenderer, m_clientWidth, m_clientHeight);
					m_pCurrentLevel->ContructLevel();
					m_levelSelection = LS_LEVEL02;
				}
				break;
				case LS_LEVEL02:
				{
					m_pCurrentLevel->DestroyLevel();
					ReleasePtr(m_pCurrentLevel);
					m_pCurrentLevel = new Level_03(m_pGDIRenderer, m_clientWidth, m_clientHeight);
					m_pCurrentLevel->ContructLevel();
					m_levelSelection = LS_LEVEL03;
				}
				break;
				case LS_LEVEL03:
				{
					m_pCurrentLevel->DestroyLevel();
					ReleasePtr(m_pCurrentLevel);
					m_pCurrentLevel = new Level_01(m_pGDIRenderer, m_clientWidth, m_clientHeight);
					m_pCurrentLevel->ContructLevel();
					m_levelSelection = LS_LEVEL01;
				}
				break;
			}	// End Switch
		}
	}
}

void Application::Draw()
{
	// Render calls when using the DX10 Renderer
	if (m_pDX10_Renderer != 0)
	{
		// Get the Renderer Ready to receive new data
		m_pDX10_Renderer->StartRender();

		m_pCube->Render();

		// Tell the Renderer the data input is over and present the outcome
		m_pDX10_Renderer->EndRender();
	}

	// Render calls when using the GDI Renderer
	if (m_pGDIRenderer != 0)
	{
		m_pGDIRenderer->BeginRender();

		m_pCurrentLevel->Render();

		if (m_mouseDown == true)
		{
			DrawCutLine();
		}

		RenderInstructions();

		m_pGDIRenderer->EndRender();
	}
}

void Application::HandleInput()
{
	if (m_pKeyDown[VK_ESCAPE])
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

	if (m_pDX10_Renderer != 0)
	{
		if (m_pKeyDown[VK_F1])
		{
			m_pDX10_Renderer->ToggleFullscreen();
			m_pKeyDown[VK_F1] = false;
		}
		if (m_pKeyDown[VK_F2])
		{
			m_pDX10_Renderer->ToggleFillMode();
			m_pKeyDown[VK_F2] = false;
		}

		// Camera Inputs
		if (m_pKeyDown[0x57]) // W Key
		{
			m_pCamera->MoveForwards(1.0f);
		}
		if (m_pKeyDown[0x53]) // S Key
		{
			m_pCamera->MoveForwards(-1.0f);
		}
		if (m_pKeyDown[0x41])	// A Key
		{
			m_pCamera->Strafe(-1.0f);
		}
		if (m_pKeyDown[0x44])	// D Key
		{
			m_pCamera->Strafe(1.0f);
		}
		if (m_pKeyDown[0x45])	// E Key
		{
			m_pCamera->Fly(1.0f);
		}
		if (m_pKeyDown[0x51])	// Q Key
		{
			m_pCamera->Fly(-1.0f);
		}

		if (m_pKeyDown[VK_LEFT])	// Left Arrow Key
		{
			m_pCamera->RotateYaw(-1.0f);
		}
		if (m_pKeyDown[VK_RIGHT])	// Right Arrow Key
		{
			m_pCamera->RotateYaw(1.0f);
		}
		if (m_pKeyDown[VK_UP])	// Up Arrow Key
		{
			m_pCamera->RotatePitch(-1.0f);
		}
		if (m_pKeyDown[VK_DOWN])	// Down Arrow Key
		{
			m_pCamera->RotatePitch(1.0f);
		}
	}

	if (m_levelSelection != LS_NONE)
	{
		if (m_pKeyDown[0x52])	// R Key
		{
			m_pCurrentLevel->ResetLevel();
		}

		if (m_pKeyDown[VK_F1])	// Load the First level
		{
			m_pCurrentLevel->DestroyLevel();
			ReleasePtr(m_pCurrentLevel);
			m_pCurrentLevel = new Level_01(m_pGDIRenderer, m_clientWidth, m_clientHeight);
			m_pCurrentLevel->ContructLevel();
			m_levelSelection = LS_LEVEL01;
		}
		if (m_pKeyDown[VK_F2])	// Load the Second level
		{
			m_pCurrentLevel->DestroyLevel();
			ReleasePtr(m_pCurrentLevel);
			m_pCurrentLevel = new Level_02(m_pGDIRenderer, m_clientWidth, m_clientHeight);
			m_pCurrentLevel->ContructLevel();
			m_levelSelection = LS_LEVEL02;
		}
		if (m_pKeyDown[VK_F3])	// Load the Third level
		{
			m_pCurrentLevel->DestroyLevel();
			ReleasePtr(m_pCurrentLevel);
			m_pCurrentLevel = new Level_03(m_pGDIRenderer, m_clientWidth, m_clientHeight);
			m_pCurrentLevel->ContructLevel();
			m_levelSelection = LS_LEVEL03;
		}
	}
}

void Application::CutRope()
{
	m_pCurrentLevel->CutRope(m_firstMousePos, m_secondMousePos);
}

void Application::DrawCutLine()
{
	m_pGDIRenderer->RenderLine(m_firstMousePos, m_secondMousePos, colorRef::RED);
}

void Application::RenderInstructions()
{
	int yPos = 0;

	m_pGDIRenderer->WriteLine("INSTRUCTIONS", 1080, Increment(&yPos, 150), colorRef::WHITE);
	
	m_pGDIRenderer->WriteLine("Use the Mouse Left click to cut the", 1030, Increment(&yPos, 15), colorRef::WHITE);
	m_pGDIRenderer->WriteLine("purple ropes. Hold down to draw a", 1030, Increment(&yPos, 15), colorRef::WHITE);
	m_pGDIRenderer->WriteLine("cutting line and release to cut.", 1030, Increment(&yPos, 15), colorRef::WHITE);
	m_pGDIRenderer->WriteLine("The aim of each level is to get the", 1030, Increment(&yPos, 30), colorRef::WHITE);
	m_pGDIRenderer->WriteLine("yellow gem to the green win zone.", 1030, Increment(&yPos, 15), colorRef::WHITE);

	m_pGDIRenderer->WriteLine("KEY CODES", 1080, Increment(&yPos, 80), colorRef::WHITE);
	m_pGDIRenderer->WriteLine("[R]     -> Reset current Level", 1030, Increment(&yPos, 15), colorRef::WHITE);
	m_pGDIRenderer->WriteLine("[F1]   -> Skip to Level 01", 1030, Increment(&yPos, 15), colorRef::WHITE);
	m_pGDIRenderer->WriteLine("[F2]   -> Skip to Level 02", 1030, Increment(&yPos, 15), colorRef::WHITE);
	m_pGDIRenderer->WriteLine("[F3]   -> Skip to Level 03", 1030, Increment(&yPos, 15), colorRef::WHITE);
	m_pGDIRenderer->WriteLine("[Esc] -> Quit Game", 1030, Increment(&yPos, 15), colorRef::WHITE);

	m_pGDIRenderer->WriteLine("COLOR LEGEND", 1080, Increment(&yPos, 80), colorRef::WHITE);
	m_pGDIRenderer->WriteLine("Gem", 1030, Increment(&yPos, 15), colorRef::YELLOW);
	m_pGDIRenderer->WriteLine("Win Zone", 1030, Increment(&yPos, 15), colorRef::GREEN);
	m_pGDIRenderer->WriteLine("Cuttable Rope", 1030, Increment(&yPos, 15), colorRef::PURPLE);
	m_pGDIRenderer->WriteLine("Uncuttable Rope", 1030, Increment(&yPos, 15), colorRef::GREY);
	m_pGDIRenderer->WriteLine("Interactive Object", 1030, Increment(&yPos, 15), colorRef::CYAN);
	m_pGDIRenderer->WriteLine("Enemy Object", 1030, Increment(&yPos, 15), colorRef::RED);
	m_pGDIRenderer->WriteLine("Breakable Object", 1030, Increment(&yPos, 15), colorRef::PINK);
	m_pGDIRenderer->WriteLine("Spring Object", 1030, Increment(&yPos, 15), colorRef::DARKBLUE);
}

int Application::Increment(int* _value, int _amount)
{
	*_value += _amount;
	return *_value;
}
















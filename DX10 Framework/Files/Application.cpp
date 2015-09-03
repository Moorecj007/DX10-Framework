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
		//Create a Console window
		if (AllocConsole())
		{
			freopen_s(&g_file, "conout$", "w", stdout);
			HWND hWnd = GetConsoleWindow();
			ShowWindow(hWnd, 1);
		}
	#endif

	int clientWidth = 1000;
	int clientHeight = 1000;

	// Create the Application 
	Application* pApp = Application::GetInstance();
	
	if (pApp->CreateWindowApp(clientWidth, clientHeight, _hInstance) == true)
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
		s_pApp = new Application();
	}
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
		VALIDATE(m_pGDIRenderer->Initialise(m_hWnd, _hInstance, m_clientWidth, m_clientHeight));

		// Initialise the Physics system
		m_pPhysics2D = new PhysicsWorld_2D();
		VALIDATE(m_pPhysics2D->Initialise(40.0f));
	
		// Initialise the Objects
		Physics_Body_2D* pTempBody;
		TPhysicsProperties physProps;
		v2float* pPoints;
		GDI_Obj_Generic* m_pTempObject;

		GDI_Obj_Generic* m_pBackground = new GDI_Obj_Polygon(m_pGDIRenderer);
		pPoints = new v2float[4];
		pPoints[0] = { -1000.0f, -1000.0f };
		pPoints[1] = { 1000.0f, -1000.0f };
		pPoints[2] = { 1000.0f, 1000.0f };
		pPoints[3] = { -1000.0f, 1000.0f };
		ZeroMemory(&physProps, sizeof(physProps));
		physProps.circleObject = false;
		physProps.pPoints = pPoints;
		physProps.size = 4;
		physProps.pos = { 0, 0 };
		physProps.collisionType = CT_BACKGROUND;
		pTempBody = m_pPhysics2D->CreatePhysicsObject(physProps);
		VALIDATE(m_pBackground->Initialise(pTempBody, 0xFFFFFF));
		m_staticObjects.push_back(m_pBackground);

		m_pTempObject = new GDI_Obj_Polygon(m_pGDIRenderer);
		pPoints = new v2float[4];
		pPoints[0] = { -100.0f, -25.0f};
		pPoints[1] = {  100.0f, -25.0f };
		pPoints[2] = {  100.0f,  25.0f };
		pPoints[3] = { -100.0f,  25.0f };
		ZeroMemory(&physProps, sizeof(physProps));
		physProps.circleObject = false;
		physProps.pPoints = pPoints;
		physProps.size = 4;
		physProps.pos = { 500, 500 };
		physProps.density = 1.0f;
		physProps.friction = 0.3f;
		physProps.collisionType = CT_BREAKABLE;
		physProps.collideWith = (CT_STATIC | CT_DYNAMIC | CT_BREAKABLE);
		pTempBody = m_pPhysics2D->CreatePhysicsObject(physProps);
		VALIDATE(m_pTempObject->Initialise(pTempBody, 0xFF0000));
		m_breakableObjects.push_back(m_pTempObject);

		m_pPhysics2D->CreateRevoluteJoint(m_pBackground->GetPhysicsBody(), m_pTempObject->GetPhysicsBody(), { 405, 500 }, false);
		m_pPhysics2D->CreateRevoluteJoint(m_pBackground->GetPhysicsBody(), m_pTempObject->GetPhysicsBody(), { 595, 500 }, false);

		m_pTempObject = new GDI_Obj_Polygon(m_pGDIRenderer);
		pPoints = new v2float[4];
		pPoints[0] = { -20.0f, 20.0f };
		pPoints[1] = { 0.0f, -15.0f };
		pPoints[2] = { 20.0f, 20.0f };
		ZeroMemory(&physProps, sizeof(physProps));
		physProps.circleObject = false;
		physProps.pPoints = pPoints;
		physProps.size = 3;
		physProps.pos = { 500, 700 };
		physProps.friction = 0.3f;
		physProps.collisionType = CT_STATIC;
		physProps.collideWith = (CT_STATIC | CT_DYNAMIC | CT_BREAKABLE);
		pTempBody = m_pPhysics2D->CreatePhysicsObject(physProps);
		VALIDATE(m_pTempObject->Initialise(pTempBody, 0xFF0000));
		m_breakableObjects.push_back(m_pTempObject);

		GDI_Obj_Generic* m_pCircle = new GDI_Obj_Circle(m_pGDIRenderer);
		ZeroMemory(&physProps, sizeof(physProps));
		physProps.circleObject = true;
		physProps.pos = { 500, 300 };
		physProps.radius = 25.0f;
		physProps.density = 1.0f;
		physProps.friction = 0.3f;
		physProps.collisionType = CT_DYNAMIC;
		physProps.collideWith = (CT_STATIC | CT_DYNAMIC | CT_BREAKABLE);
		pTempBody = m_pPhysics2D->CreatePhysicsObject(physProps);
		VALIDATE(m_pCircle->Initialise(pTempBody, 0x0000FF));
		m_dynamicObjects.push_back(m_pCircle);

		//GDI_Line* m_pLine = new GDI_Line(m_pGDIRenderer);
		//Physics_Joint_2D* pRopeJoint = m_pPhysics2D->CreateRopeJoint(m_pBackground->GetPhysicsBody(), m_pQuad->GetPhysicsBody(), { 495, 0 }, { 0, 0 }, true);
		//VALIDATE(m_pLine->Initialise(pRopeJoint, 0xFFFF00));
		//m_lines.push_back(m_pLine)
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

	ReleasePtr(m_pShader_LitTex);
	ReleasePtr(m_pCamera);
	ReleasePtr(m_pCubeMesh);
	ReleasePtr(m_pCube);

	// Delete all GDI static objects
	for (UINT i = 0; i < m_staticObjects.size(); i++)
	{
		ReleasePtr(m_staticObjects[i]);
	}
	// Delete all GDI dynamic objects
	for (UINT i = 0; i < m_dynamicObjects.size(); i++)
	{
		ReleasePtr(m_dynamicObjects[i]);
	}
	// Delete all GDI breakable objects
	for (UINT i = 0; i < m_breakableObjects.size(); i++)
	{
		ReleasePtr(m_breakableObjects[i]);
	}
	// Delete all GDI Lines
	for (UINT i = 0; i < m_lines.size(); i++)
	{
		ReleasePtr(m_lines[i]);
	}

	ReleasePtr(m_pPhysics2D);

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
		printf("%d \n", m_fps);
		m_fpsTimer -= 1.0f;
		m_fps = 0;
	}
}

void Application::Process(float _dt)
{
	HandleInput();

	if (m_pDX10_Renderer != 0)
	{
		m_pCamera->Process(_dt);
		m_pCube->Process(_dt);
		m_pShader_LitTex->SetUpPerFrame();
	}

	if (m_pGDIRenderer != 0)
	{
		m_pPhysics2D->Process();

		// Process all static objects
		for (UINT i = 0; i < m_staticObjects.size(); i++)
		{
			m_staticObjects[i]->Process(_dt);
		}
		// Process all dynamic objects
		for (UINT i = 0; i < m_dynamicObjects.size(); i++)
		{
			m_dynamicObjects[i]->Process(_dt);
		}
		// Process all breakable objects
		for (UINT i = 0; i < m_breakableObjects.size(); i++)
		{
			TBreakProperties* breakProps =  m_breakableObjects[i]->GetPhysicsBody()->GetBreakProperties();

			if (breakProps->broken == true)
			{
				std::vector<Physics_Body_2D*>* pNewBodies = m_pPhysics2D->BreakObject(m_breakableObjects[i]->GetPhysicsBody());

				// TO DO
				// Destroy this object
				// Create new objects

				breakProps->broken = false;
			}

			m_breakableObjects[i]->Process(_dt);
		}
		// Process all lines
		for (UINT i = 0; i < m_lines.size(); i++)
		{
			m_lines[i]->Process(_dt);
		}

	}
}

void Application::Draw()
{
	if (m_pDX10_Renderer != 0)
	{
		// Get the Renderer Ready to receive new data
		m_pDX10_Renderer->StartRender();

		m_pCube->Render();

		// Tell the Renderer the data input is over and present the outcome
		m_pDX10_Renderer->EndRender();
	}

	if (m_pGDIRenderer != 0)
	{
		m_pGDIRenderer->BeginRender();

		// Render all static objects
		for (UINT i = 0; i < m_staticObjects.size(); i++)
		{
			m_staticObjects[i]->Render();
		}
		// Render all dynamic objects
		for (UINT i = 0; i < m_dynamicObjects.size(); i++)
		{
			m_dynamicObjects[i]->Render();
		}
		// Render all breakable objects
		for (UINT i = 0; i < m_breakableObjects.size(); i++)
		{
			m_breakableObjects[i]->Render();
		}
		// Render all lines
		for (UINT i = 0; i < m_lines.size(); i++)
		{
			m_lines[i]->Render();
		}

		m_pGDIRenderer->EndRender();
	}
}

void Application::HandleInput()
{
	// Template Inputs
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
	if (m_pKeyDown[VK_ESCAPE])
	{
		if (m_pDX10_Renderer != 0)
		{
			if (m_pDX10_Renderer->GetFullScreenState() == true)
			{
				m_pDX10_Renderer->ToggleFullscreen();
			}
		}
		m_online = false;
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

void Application::SetKeyDown(int _index, bool _down)
{
	m_pKeyDown[_index] = _down;
}

















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

	#ifdef _DEBUG
		if (AllocConsole())
		{
			freopen_s(&g_file, "conout$", "w", stdout);
			SetConsoleTitleA("Debug Console");
		}
	#endif // _DEBUG

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

	// Save the client window sizes
	m_clientWidth = _clientWidth;
	m_clientHeight = _clientHeight;

	// Initialise boolean array for KeyDown to false
	m_pKeyDown = new bool[255];
	memset(m_pKeyDown, false, 255);

	VALIDATE(Initialise_DX10(_hInstance));

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
	m_pCamera = new DX10_Camera_FirstPerson();
	VALIDATE(m_pCamera->Initialise(m_pDX10_Renderer, _hInstance, m_hWnd));

	// Create the Shaders
	m_pShader_LitTex = new DX10_Shader_LitTex();
	VALIDATE(m_pShader_LitTex->Initialise(m_pDX10_Renderer));

	m_pShader_Sprite = new DX10_Shader_Sprite();
	VALIDATE(m_pShader_Sprite->Initialise(m_pDX10_Renderer, &m_hWnd));

	// Create the Meshes
	m_pMesh_Terrain = new DX10_Mesh();
	VALIDATE(m_pMesh_Terrain->InitialisePlane(m_pDX10_Renderer, 65, { 10, 10, 10 }));

	// Create the Objects
	m_pSprite_InstructionsLeft = new DXSprite();
	VALIDATE(m_pSprite_InstructionsLeft->Initialise(m_pDX10_Renderer, m_pShader_Sprite, "InstructionsRed.png", 384, 140));
	m_pSprite_InstructionsLeft->SetPosition(5, 5);

	m_pSprite_InstructionsRight = new DXSprite();
	VALIDATE(m_pSprite_InstructionsRight->Initialise(m_pDX10_Renderer, m_pShader_Sprite, "InstructionsRightRed.png", 384, 140));
	m_pSprite_InstructionsRight->SetPosition(600, 5);

	m_pObj_Terrain = new DX10_Obj_LitTex();
	VALIDATE(m_pObj_Terrain->Initialise(m_pDX10_Renderer, m_pMesh_Terrain, m_pShader_LitTex, "WaterMap.png"));

	// Create the Texture Resources for Refraction and Reflection
	m_pDX10_Renderer->CreateTextureResource(m_pRefractionTexture);
	m_pDX10_Renderer->CreateTextureResource(m_pReflectionTexture);

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
		// Release the Cameras
		ReleasePtr(m_pCamera);
		// Release the Shaders
		ReleasePtr(m_pShader_LitTex);
		ReleasePtr(m_pShader_Sprite);
		// Release the Meshes
		ReleasePtr(m_pMesh_Terrain);
		// Release the Objects
		ReleasePtr(m_pSprite_InstructionsLeft);
		ReleasePtr(m_pSprite_InstructionsRight);
		ReleasePtr(m_pObj_Terrain);
		// Release the Texture Resources
		ReleasePtr(m_pRefractionTexture);
		ReleasePtr(m_pReflectionTexture);

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

	// Limit to 60 FPS for Renderering
	if (m_deltaTick > (1.0 / 60.0f))
	{
		if (Process(m_deltaTick) == false)
		{
			// A process failed to create something
			m_online = false;
			return;
		}

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
		m_pCamera->Process(_dt);

		ProcessShaders();

		m_pObj_Terrain->Process(_dt);
	}

	return true;
}

void Application::ProcessShaders()
{
	m_pShader_LitTex->SetUpPerFrame();
	m_pShader_Sprite->Update(m_deltaTick);
}

void Application::Render()
{
	// Render calls when using the DX10 Renderer
	if (m_pDX10_Renderer != 0)
	{
		// Render the Refraction and Reflection Textures
		//RenderRefraction();
		//RenderReflection();
		
		// Tell the Renderer that the data input for the back buffer is about to commence
		m_pDX10_Renderer->StartRender();

		// Render the Objects of the Scene
		m_pObj_Terrain->Render();
		
		m_pDX10_Renderer->ApplyDepthStencilState(DS_ZDISABLED);
		m_pSprite_InstructionsLeft->Render();
		m_pSprite_InstructionsRight->Render();
		m_pDX10_Renderer->ApplyDepthStencilState(DS_NORMAL);
		
		// Tell the Renderer the data input is over and present the outcome
		m_pDX10_Renderer->EndRender();
	}
}

void Application::RenderRefraction()
{
	// Create a clip plane for the Water so nothing above it is rendered for the refraction
	D3DXPLANE clipPlane = D3DXPLANE(0.0f, -1.0f, 0.0f, 2.0f);

	// Set the Refraction Texture to be the Render Target of the Graphics Device and clear it 
	m_pRefractionTexture->SetRenderTarget();
	m_pRefractionTexture->ClearRenderTarget();

	// Render all objects that will be refracted


	// Reset the Devices Render Target to be the default back buffer on the swap chain
	m_pDX10_Renderer->ResetRenderTarget();
}

void Application::RenderReflection()
{
	// Set the Reflection Texture to be the Render Target of the Graphics Device and clear it 
	m_pReflectionTexture->SetRenderTarget();
	m_pReflectionTexture->ClearRenderTarget();

	// Create a mirror plane for the reflective surface
	D3DXPLANE mirrorPlane = { 0.0f, -1.0f, 0.0f, 0.0f };

	// Reflect the Lights across the plane so they are lighting the correct sides of the reflected objects
	m_pDX10_Renderer->ReflectLightsAcrossPlane(mirrorPlane);

	// Render all objects that will be reflected


	// Reflect the Lights back across the plane to resume normal lighting
	m_pDX10_Renderer->ReflectLightsAcrossPlane(mirrorPlane);

	// Reset the Devices Render Target to be the default back buffer on the swap chain
	m_pDX10_Renderer->ResetRenderTarget();
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
		//m_pDX10_Renderer->ToggleFullscreen();
		//UpdateClientSize();
		//
		//SetKeyDown(VK_F1, false);
	}

	if (m_pKeyDown[VK_F2])
	{
		m_pDX10_Renderer->ToggleFillMode();

		SetKeyDown(VK_F2, false);
	}

	if (m_pKeyDown[0x31])
	{
		if (m_pMesh_Terrain->DiamondSquare() == false)
		{
			MessageBoxA(m_hWnd, "Maximum iterations have been reached. Try applying the smoothing algorithm by pressing '2'", "Information", MB_OK | MB_ICONINFORMATION);
			memset(m_pKeyDown, false, 255);
		}
		SetKeyDown(0x31, false);
	}

	if (m_pKeyDown[0x32])
	{
		m_pMesh_Terrain->Smooth();
		SetKeyDown(0x32, false);
	}

	if (m_pKeyDown[0x33])
	{
		m_pMesh_Terrain->ResetPlane();
		SetKeyDown(0x33, false);
	}

	// Camera Controls
	if ((m_pKeyDown[0x41]) && !(m_pKeyDown[0x44]))
	{
		// A pressed
		m_pCamera->Strafe(-1);
	}

	if ((m_pKeyDown[0x44]) && !(m_pKeyDown[0x41]))
	{
		// D pressed
		m_pCamera->Strafe(1);
	}

	if ((m_pKeyDown[0x57]) && !(m_pKeyDown[0x53]))
	{
		// W pressed
		m_pCamera->MoveForwards(1);
	}

	if ((m_pKeyDown[0x53]) && !(m_pKeyDown[0x57]))
	{
		// S pressed
		m_pCamera->MoveForwards(-1);
	}

	if ((m_pKeyDown[0x45]) && !(m_pKeyDown[0x51]))
	{
		// E pressed
		m_pCamera->Fly(1);
	}

	if ((m_pKeyDown[0x51]) && !(m_pKeyDown[0x45]))
	{
		// Q pressed
		m_pCamera->Fly(-1);
	}

	if ((m_pKeyDown[VK_UP]) && !(m_pKeyDown[VK_DOWN]))
	{
		// Down arrow pressed
		m_pCamera->RotatePitch(-1);
	}

	if ((m_pKeyDown[VK_DOWN]) && !(m_pKeyDown[VK_UP]))
	{
		// Up arrow pressed
		m_pCamera->RotatePitch(1); 
	}

	if ((m_pKeyDown[VK_LEFT]) && !(m_pKeyDown[VK_RIGHT]))
	{
		// Left arrow pressed
		m_pCamera->RotateYaw(-1);
	}

	if ((m_pKeyDown[VK_RIGHT]) && !(m_pKeyDown[VK_LEFT]))
	{
		// Right arrow pressed
		m_pCamera->RotateYaw(1);
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

void Application::UpdateClientSize()
{
	float width = static_cast<float>(m_pDX10_Renderer->GetWidth());
	float height = static_cast<float>(m_pDX10_Renderer->GetHeight());


	float diff = max(width, height) - min(width, height);

	if (max(width, height) == width)
	{
		m_pSprite_InstructionsLeft->SetPosition((diff / 2.0f), 0.0f);
		m_pSprite_InstructionsRight->SetPosition(width - 400.0f, 0.0f);
	}
	else
	{
		m_pSprite_InstructionsLeft->SetPosition(0.0f, (diff / 2.0f));
		m_pSprite_InstructionsRight->SetPosition(0.0f, height - 400.0f);
	}
}
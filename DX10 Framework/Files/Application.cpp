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
	int clientWidth = 900;
	int clientHeight = 900;

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
			pApp->SelectSliders(true);
		}
		break;
		case WM_MOUSEMOVE:
		{
			POINT mousePos;
			mousePos.x = GET_X_LPARAM(_lParam);
			mousePos.y = GET_Y_LPARAM(_lParam);

			pApp->UpdateMousePos(mousePos);
		}
		break;
		case WM_LBUTTONUP:
		{
			pApp->SetMouseDown(false);	
			pApp->SelectSliders(false);
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

	m_mouseDown = false;
	m_initialRayCast = false;

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

	
	//--------------------------------------------------------------
	// Initialise the Cameras
	//--------------------------------------------------------------

	m_pCamera = new DX10_Camera_FirstPerson();
	VALIDATE(m_pCamera->Initialise(m_pDX10_Renderer, _hInstance, m_hWnd));

	//--------------------------------------------------------------
	// Create the Shaders
	//--------------------------------------------------------------

	m_pShader_LitTex = new DX10_Shader_LitTex();
	VALIDATE(m_pShader_LitTex->Initialise(m_pDX10_Renderer));

	m_pShader_Sprite = new DX10_Shader_Sprite();
	VALIDATE(m_pShader_Sprite->Initialise(m_pDX10_Renderer, &m_hWnd));

	m_pShader_ShadowMap = new DX10_Shader_ShadowMap();
	VALIDATE(m_pShader_ShadowMap->Initialise(m_pDX10_Renderer));

	m_pShader_Shadow = new DX10_Shader_Shadow();
	VALIDATE(m_pShader_Shadow->Initialise(m_pDX10_Renderer));

	m_pShader_ShadowSoft = new DX10_Shader_ShadowSoft();
	VALIDATE(m_pShader_ShadowSoft->Initialise(m_pDX10_Renderer));

	m_pShader_Blur = new DX10_Shader_Blur();
	VALIDATE(m_pShader_Blur->Initialise(m_pDX10_Renderer));

	//--------------------------------------------------------------
	// Create the Meshes
	//--------------------------------------------------------------

	m_pMesh_Floor = new DX10_Mesh();
	VALIDATE(m_pMesh_Floor->Initialise(m_pDX10_Renderer, MT_FINITEPLANE, { 100.0f, 100.0f, 100.0f }));

	m_pMesh_Sphere = new DX10_Mesh();
	VALIDATE(m_pMesh_Sphere->Initialise(m_pDX10_Renderer, MT_SPHERE, { 5.0f, 5.0f, 5.0f }));

	m_pMesh_Capsule = new DX10_Mesh();
	VALIDATE(m_pMesh_Capsule->Initialise(m_pDX10_Renderer, MT_CAPSULE, { 3.0f, 3.0f, 3.0f }));

	m_pMesh_Pyramid = new DX10_Mesh();
	VALIDATE(m_pMesh_Pyramid->Initialise(m_pDX10_Renderer, MT_PYRAMID, { 10.0f, 10.0f, 10.0f }));

	//--------------------------------------------------------------
	// Create the sprites
	//--------------------------------------------------------------

	m_pSprite_InstructionsLeft = new DXSprite();
	VALIDATE(m_pSprite_InstructionsLeft->Initialise(m_pDX10_Renderer, m_pShader_Sprite, "Cloth_Instructions_Left.png", 300, 900));
	m_pSprite_InstructionsLeft->SetPosition(10, 0);

	m_pSprite_InstructionsRight = new DXSprite();
	VALIDATE(m_pSprite_InstructionsRight->Initialise(m_pDX10_Renderer, m_pShader_Sprite, "Cloth_Instructions_Right.png", 300, 900));
	m_pSprite_InstructionsRight->SetPosition(1050, 0);

	//--------------------------------------------------------------
	// Create the Sliders
	//--------------------------------------------------------------

	//m_pSlider_WindSpeed = new TSliderBar();
	//VALIDATE(m_pSlider_WindSpeed->Initialise(m_pDX10_Renderer, m_pShader_Sprite, { 1150, 400 }, 140, 40));
	//
	//m_pSlider_ClothWidth = new TSliderBar();
	//VALIDATE(m_pSlider_ClothWidth->Initialise(m_pDX10_Renderer, m_pShader_Sprite, { 100, 535 }, 140, 40));
	//
	//m_pSlider_ClothHeight = new TSliderBar();
	//VALIDATE(m_pSlider_ClothHeight->Initialise(m_pDX10_Renderer, m_pShader_Sprite, { 100, 615 }, 140, 40));
	//
	//m_pSlider_HookAmount = new TSliderBar();
	//VALIDATE(m_pSlider_HookAmount->Initialise(m_pDX10_Renderer, m_pShader_Sprite, { 100, 690 }, 140, 40));
	//
	//m_pSlider_BlastRadius = new TSliderBar();
	//VALIDATE(m_pSlider_BlastRadius->Initialise(m_pDX10_Renderer, m_pShader_Sprite, { 1150, 715 }, 140, 40));

	//--------------------------------------------------------------
	// Create the Objects
	//--------------------------------------------------------------

	m_pObj_Floor = new DX10_Obj_LitTex();
	VALIDATE(m_pObj_Floor->Initialise(m_pDX10_Renderer, m_pMesh_Floor, m_pShader_LitTex, "Dragon.png"));
	m_pObj_Floor->SetPosition({ 0.0f, 0.0f, 0.0f });

	m_pObj_Sphere = new DX10_Obj_LitTex();
	VALIDATE(m_pObj_Sphere->Initialise(m_pDX10_Renderer, m_pMesh_Sphere, m_pShader_LitTex, "WaterMap.png"));
	m_pObj_Sphere->SetPosition({ 0.0f, 4.0f, 7.0f });

	m_pObj_Capsule = new DX10_Obj_LitTex();
	VALIDATE(m_pObj_Capsule->Initialise(m_pDX10_Renderer, m_pMesh_Capsule, m_pShader_LitTex, "WaterMap.png"));
	m_pObj_Capsule->SetPosition({ -10.0f, 4.0f, 6.0f });

	m_pObj_Pyramid = new DX10_Obj_LitTex();
	VALIDATE(m_pObj_Pyramid->Initialise(m_pDX10_Renderer, m_pMesh_Pyramid, m_pShader_LitTex, "WaterMap.png"));
	m_pObj_Pyramid->SetPosition({ 10.0f, 4.0f, 7.0f });

	// Set the beginning ratios of the slider bars
	//m_pSlider_WindSpeed->SetRatio(0.0f);
	//m_pSlider_ClothWidth->SetRatio(0.5f);
	//m_pSlider_ClothHeight->SetRatio(0.5f);
	//m_pSlider_HookAmount->SetRatio(0.1f);
	//m_pSlider_BlastRadius->SetRatio(0.4f);

	//--------------------------------------------------------------
	// Create Shadowing variables
	//--------------------------------------------------------------

	for (int i = 0; i < 2; i++)
	{
		VALIDATE(m_pDX10_Renderer->CreateShadowMap(m_pShadowMap[i], 1024, 1024, 4.0f));
	}

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
		ReleasePtr(m_pShader_ShadowMap);
		ReleasePtr(m_pShader_Shadow);
		ReleasePtr(m_pShader_ShadowSoft);
		ReleasePtr(m_pShader_Blur);
		// Release the Meshes
		ReleasePtr(m_pMesh_Floor);
		ReleasePtr(m_pMesh_Sphere);
		ReleasePtr(m_pMesh_Capsule);
		ReleasePtr(m_pMesh_Pyramid);
		// Release the Sprites
		ReleasePtr(m_pSprite_InstructionsLeft);
		ReleasePtr(m_pSprite_InstructionsRight);
		//ReleasePtr(m_pSlider_WindSpeed);
		//ReleasePtr(m_pSlider_ClothWidth);
		//ReleasePtr(m_pSlider_ClothHeight);
		//ReleasePtr(m_pSlider_HookAmount);
		//ReleasePtr(m_pSlider_BlastRadius);
		// Release the Objects
		ReleasePtr(m_pObj_Floor);
		ReleasePtr(m_pObj_Sphere);
		ReleasePtr(m_pObj_Capsule);
		ReleasePtr(m_pObj_Pyramid);
		// Release Shadowing Variables 	
		for (int i = 0; i < 2; i++)
		{
			ReleasePtr(m_pShadowMap[i]);
		}

		// Release the Renderers resources
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

	// Limit to 30 FPS for Renderering Physics
	if (m_deltaTick > (1.0 / 30.0f))
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

		m_pObj_Floor->Process(_dt);
		m_pObj_Sphere->Process(_dt);
		m_pObj_Capsule->Process(_dt);
		m_pObj_Pyramid->Process(_dt);
	}
	return true;
}

void Application::ProcessShaders()
{
	m_pShader_LitTex->SetUpPerFrame();
	m_pShader_Sprite->Update(m_deltaTick);
	m_pShader_ShadowMap->SetUpPerFrame();
	m_pShader_Shadow->SetUpPerFrame();
}

void Application::Render()
{
	// Render calls when using the DX10 Renderer
	if (m_pDX10_Renderer != 0)
	{		
		m_pDX10_Renderer->RestoreDefaultRenderStates();
		m_pDX10_Renderer->CreateLightPosForShadowing(50.0f);

		int lightCount = m_pDX10_Renderer->GetLightCount();
		TLight* pLightArray = m_pDX10_Renderer->GetActiveLights();

		for (int i = 0; i < lightCount; i++)
		{
			D3DXMATRIX matLightView;
			D3DXVECTOR3 lightPos = { pLightArray[i].pos_range.x, pLightArray[i].pos_range.y, pLightArray[i].pos_range.z };
			D3DXMatrixLookAtLH(&matLightView, &lightPos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

			D3DXMATRIX camView = *m_pDX10_Renderer->GetViewMatrix();

			// Render the Shadow Map as a depth stencil
			m_pShadowMap[i]->StartRender();

			m_pShader_ShadowMap->Render(m_pObj_Sphere, matLightView);
			m_pShader_ShadowMap->Render(m_pObj_Capsule, matLightView);
			m_pShader_ShadowMap->Render(m_pObj_Pyramid, matLightView);

			m_pShadowMap[i]->EndRender();

			// Render the scene with shadows to a black and white image
			m_pShadowMap[i]->GetShadowedTex()->SetRenderTarget();
			m_pShadowMap[i]->GetShadowedTex()->ClearRenderTarget(0, 0, 0, 1);
			m_pShader_Shadow->Render(m_pObj_Floor, &pLightArray[i], matLightView, m_pShadowMap[i]);
			m_pShader_Shadow->Render(m_pObj_Sphere, &pLightArray[i], matLightView, m_pShadowMap[i]);
			m_pShader_Shadow->Render(m_pObj_Capsule, &pLightArray[i], matLightView, m_pShadowMap[i]);
			m_pShader_Shadow->Render(m_pObj_Pyramid, &pLightArray[i], matLightView, m_pShadowMap[i]);

			m_pDX10_Renderer->ApplyDepthStencilState(DS_ZDISABLED);
			m_pShader_Blur->Render(m_pShadowMap[i]);
			m_pDX10_Renderer->ApplyDepthStencilState(DS_NORMAL);
		}

		// Tell the Renderer that the data input for the back buffer is about to commence
		m_pDX10_Renderer->StartRender();

		// Render the Objects of the Scene	
		m_pShader_ShadowSoft->Render(m_pObj_Floor, m_pShadowMap[0], m_pShadowMap[1]);
		m_pShader_ShadowSoft->Render(m_pObj_Sphere, m_pShadowMap[0], m_pShadowMap[1]);
		m_pShader_ShadowSoft->Render(m_pObj_Capsule, m_pShadowMap[0], m_pShadowMap[1]);
		m_pShader_ShadowSoft->Render(m_pObj_Pyramid, m_pShadowMap[0], m_pShadowMap[1]);
		

		m_pDX10_Renderer->ApplyDepthStencilState(DS_ZDISABLED);
		//m_pSprite_InstructionsLeft->Render();
		//m_pSprite_InstructionsRight->Render();
		//m_pSlider_WindSpeed->Render();
		//m_pSlider_ClothWidth->Render();
		//m_pSlider_ClothHeight->Render();
		//m_pSlider_HookAmount->Render();
		//m_pSlider_BlastRadius->Render();
		m_pDX10_Renderer->ApplyDepthStencilState(DS_NORMAL);
		
		// Tell the Renderer the data input is over and present the outcome
		m_pDX10_Renderer->EndRender();
	}
}

bool Application::HandleInput()
{
	// Exit the application
	if (m_pKeyDown[VK_ESCAPE])
	{
		ExitApp();
	}

	// Toggling on Full screen
	//if (m_pKeyDown[VK_F1])
	//{
	//	m_pDX10_Renderer->ToggleFullscreen();
	//	UpdateClientSize();
	//	
	//	SetKeyDown(VK_F1, false);
	//}

	// Toggling on Wire frame mode
	if (m_pKeyDown[VK_F1])
	{
		m_pDX10_Renderer->ToggleFillMode();

		SetKeyDown(VK_F1, false);
	}

	// Camera Controls
	if ((m_pKeyDown[input::VK_A]) && !(m_pKeyDown[input::VK_D]))
	{
		// A Key pressed
		m_pCamera->Strafe(-1);
	}

	if ((m_pKeyDown[input::VK_D]) && !(m_pKeyDown[input::VK_A]))
	{
		// D Key pressed
		m_pCamera->Strafe(1);
	}

	if ((m_pKeyDown[input::VK_W]) && !(m_pKeyDown[input::VK_S]))
	{
		// W Key pressed
		m_pCamera->MoveForwards(1);
	}

	if ((m_pKeyDown[input::VK_S]) && !(m_pKeyDown[input::VK_W]))
	{
		// S Key pressed
		m_pCamera->MoveForwards(-1);
	}

	if ((m_pKeyDown[input::VK_E]) && !(m_pKeyDown[input::VK_Q]))
	{
		// E Key pressed
		m_pCamera->Fly(1);
	}

	if ((m_pKeyDown[input::VK_Q]) && !(m_pKeyDown[input::VK_E]))
	{
		// Q Key pressed
		m_pCamera->Fly(-1);
	}

	if ((m_pKeyDown[VK_UP]) && !(m_pKeyDown[VK_DOWN]) && !(m_pKeyDown[VK_CONTROL]))
	{
		// Down arrow pressed
		m_pCamera->RotatePitch(-1);
	}

	if ((m_pKeyDown[VK_DOWN]) && !(m_pKeyDown[VK_UP]) && !(m_pKeyDown[VK_CONTROL]))
	{
		// Up arrow pressed
		m_pCamera->RotatePitch(1);
	}

	if ((m_pKeyDown[VK_LEFT]) && !(m_pKeyDown[VK_RIGHT]) && !(m_pKeyDown[VK_CONTROL]))
	{
		// Left arrow pressed
		m_pCamera->RotateYaw(-1);
	}

	if ((m_pKeyDown[VK_RIGHT]) && !(m_pKeyDown[VK_LEFT]) && !(m_pKeyDown[VK_CONTROL]))
	{
		// Right arrow pressed
		m_pCamera->RotateYaw(1);
	}
	
	if (m_pKeyDown[input::VK_Z])
	{
		m_pCamera->Reset();
		SetKeyDown(input::VK_Z, false);
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

void Application::UpdateMousePos(POINT _mousePos)
{
	m_mousePosActual.x = (float)_mousePos.x;
	m_mousePosActual.y = (float)_mousePos.y;

	// Change the mouse coordinates into -1 to +1 range
	m_mousePos.x = ((float)(2.0f * _mousePos.x) / (float)m_pDX10_Renderer->GetWidth()) - 1.0f;
	m_mousePos.y = ((float)(2.0f * _mousePos.y) / (float)m_pDX10_Renderer->GetHeight()) - 1.0f; 

	// Flip the Y axis around to that positive Y goes up ( Screen coordinates use top left as (0,0))
	m_mousePos.y *= -1.0f;
}

void Application::SelectSliders(bool _selected)
{
	//m_pSlider_WindSpeed->SelectSlider(m_mousePosActual, _selected);
	//m_pSlider_ClothWidth->SelectSlider(m_mousePosActual, _selected);
	//m_pSlider_ClothHeight->SelectSlider(m_mousePosActual, _selected);
	//m_pSlider_HookAmount->SelectSlider(m_mousePosActual, _selected);
	//m_pSlider_BlastRadius->SelectSlider(m_mousePosActual, _selected);
}


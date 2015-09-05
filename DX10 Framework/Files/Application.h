/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Application.h
* Description : The core of the Application is run here. The link between all components.
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

// Initialization
#include "App_Macro.h"

// Local Includes
#include "Utility\Utilities.h"
#include "Utility\Timer.h"
#include "DX10\DX10.h"
#include "GDI\GDI.h"
#include "Physics\Physics_2D.h"

class Application
{
public:
	
	/***********************
	* WindowProc: Process the window
	* @author: Callan Moore
	* @Parameter: _hWnd: Handle to the Window sending the message
	* @Parameter: _uiMsg: The message ID being sent
	* @Parameter: _wParam: Additional detail about the message being sent
	* @Parameter: _lParam: Additional detail about the message being sent
	* @return: LRESULT: The resulting value
	********************/
	static LRESULT CALLBACK WindowProc(HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam);

	/***********************
	* CreateWindowApp: Creates the window for the Application
	* @author: Callan Moore
	* @Parameter: _clientWidth: Width of the screen
	* @Parameter: _clientHeight: Height of the screen
	* @Parameter: _hInstance: The instance of the application
	* @return: bool: Successful or not
	********************/
	bool CreateWindowApp(int _clientWidth, int _clientHeight, HINSTANCE _hInstance);

	/***********************
	* Execute: Runs the main loop of the Application
	* @author: Callan Moore
	* @return: int: Integer containing details about the last message resulting it termination
	********************/
	int Execute();

	// Constructors / Destructors
	/***********************
	* GetInstance: Returns the singleton instance of the Application, if it doesn't exist creates it.
	* @author: Callan Moore
	* @return: CApplication*: The current instance of the Application
	********************/
	static Application* GetInstance();
	
	/***********************
	* Initialise: Initialise the Application
	* @author: Callan Moore
	* @parameter: _clientWidth: Width of the client window
	* @parameter: _clientHeight: Height of the client window
	* @Parameter: _hInstance: Handle to the current instance
	* @return: bool: Successful or not
	********************/
	bool Initialise(int _clientWidth, int _clientHeight, HINSTANCE _hInstance);

	/***********************
	* ~CApplication: Default Destructor for Application class
	* @author: Callan Moore
	********************/
	~Application();

	/***********************
	* DestroyInstance: Deletes the instance of the Application.
	* @author: Callan Moore
	* @return: void
	********************/
	static void DestroyInstance();

	/***********************
	* ShutDown: Shuts down and releases all memory created by Application
	* @author: Callan Moore
	* @return: void
	********************/
	void ShutDown();

	/***********************
	* ExecuteOneFrame: Executes one frame of the Application
	* @author: Callan Moore
	* @return: void
	********************/
	void ExecuteOneFrame();

	/***********************
	* Process: Process the Application
	* @author: Callan Moore
	* @parameter: _dt: The current delta tick
	* @return: void
	********************/
	void Process(float _dt);

	/***********************
	* Draw: Draw the Application components
	* @author: Callan Moore
	* @return: void
	********************/
	void Draw();

	/***********************
	* HandleInput: Handle all input for the application
	* @author: Callan Moore
	* @return: void
	********************/
	void HandleInput();

	/***********************
	* SetKeyDown: Sets the Key down boolean for input Key
	* @author: Callan Moore
	* @parameter: _index: Index of the Key to set
	* @parameter: _down: True/false setting
	* @return: void
	********************/
	void SetKeyDown(int _index, bool _down);

private:
	// Preventing copies and extra constructions
	Application();
	Application(const Application& _kr);
	Application& operator= (const Application& _kr);

private:
	// Singleton Instance
	static Application* s_pApp;

	// Window Variables
	HWND m_hWnd;
	int m_clientWidth;
	int m_clientHeight;
	bool m_online;
	
	// Timer Variables
	Timer* m_pTimer;
	float m_deltaTick;
	int m_fps;
	float m_fpsTimer; 

	// Input Variables
	bool* m_pKeyDown;

	// Renderer Variables
	DX10_Renderer* m_pDX10_Renderer;
	GDI_Renderer* m_pGDIRenderer;

	// Camera
	DX10_Camera_FirstPerson* m_pCamera;

	// Objects
	DX10_Obj_Generic* m_pCube;
	GDI_Obj_Generic* m_pBackground;
	std::vector<GDI_Obj_Generic*> m_staticObjects;
	std::vector<GDI_Obj_Generic*> m_dynamicObjects;
	std::vector<GDI_Obj_Generic*> m_breakableObjects;
	std::vector<GDI_Obj_Group*> m_groupObjects;
	std::vector<GDI_Pulley*> m_pulleyObjects;
	std::vector<GDI_Spring*> m_springObjects;
	std::vector<GDI_Line*> m_lines;

	// Meshes
	DX10_Mesh_Generic* m_pCubeMesh;
	
	// Shaders
	DX10_Shader_LitTex* m_pShader_LitTex;

	// Physics
	PhysicsWorld_2D* m_pPhysics2D;

};

#endif // __APPLICATION_H__


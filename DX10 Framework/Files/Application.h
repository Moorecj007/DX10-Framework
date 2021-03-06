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

// Library Files
#pragma comment(lib, "Winmm.lib")

// Defines and Macros
#define WIN32_LEAN_AND_MEAN
#define WINDOW_CLASS_NAME L"DX10 FRAMEWORK"

#ifdef _DEBUG
// Visual Leak Detector to be run only if in DEBUG mode
#pragma comment(lib, "vld.lib")
#include <vld.h>
#define D3D_DEBUG_INFO
#endif // _DEBUG

// Local Includes
#include "Utility\Timer.h"
#include "DX10\DX10.h"
#include "DX10\DX10\2D Objects\DX10_UI_Elements.h"
#include "Physics\Physics_3D.h"

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
	* Initialise_DX10: Initialise the DX10 for the Application
	* @author: Callan Moore
	* @Parameter: _hInstance: Handle to the current instance
	* @return: bool: Successful or not
	********************/
	bool Initialise_DX10(HINSTANCE _hInstance);

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
	* @return: bool: Successful or not
	********************/
	bool Process(float _dt);

	/***********************
	* ProcessShaders: Process all the Shaders to set up all once per frame variables
	* @author: Callan Moore
	* @return: void
	********************/
	void ProcessShaders();

	/***********************
	* Render: Render the Application components
	* @author: Callan Moore
	* @return: void
	********************/
	void Render();
	
	/***********************
	* RenderRefraction: Render the Refraction of the scene to the Texture Resource
	* @author: Callan Moore
	* @return: void
	********************/
	void RenderRefraction();
	
	/***********************
	* RenderReflection: Render the Reflection of the scene to the Texture Resource
	* @author: Callan Moore
	* @return: void
	********************/
	void RenderReflection();

	/***********************
	* HandleInput: Handle all input for the application
	* @author: Callan Moore
	* @return: Successful or not
	********************/
	bool HandleInput();

	/***********************
	* SetKeyDown: Sets the Key down boolean for input Key
	* @author: Callan Moore
	* @parameter: _index: Index of the Key to set
	* @parameter: _down: True/false setting
	* @return: void
	********************/
	void SetKeyDown(int _index, bool _down) { m_pKeyDown[_index] = _down; };

	/***********************
	* SetMouseDown: Set the state of the mouse button being clicked
	* @author: Callan Moore
	* @parameter: _mouseDown: The new state of the mouse button
	* @return: void
	********************/
	void SetMouseDown(bool _mouseDown) { m_mouseDown = _mouseDown; };

	/***********************
	* UpdateMousePos: Update the stored mouse position
	* @author: Callan Moore
	* @parameter: _mousePos: The new mouse position
	* @return: void
	********************/
	void UpdateMousePos(POINT _mousePos);

private:
	// Preventing copies and extra constructions
	Application() {};
	Application(const Application& _kr);
	Application& operator= (const Application& _kr);

	/*******************
	-> Exits the application
	@author:	Juran Griffith.
	@parameter:	None.
	@return:	void
	********************/
	void ExitApp();
	
	/***********************
	* UpdateClientSize: Update the client size for all components that use screen size
	* @author: Callan Moore
	* @return: void
	********************/
	void UpdateClientSize();

	/***********************
	* SelectSliders: Update all the slider selected states
	* @author: Callan Moore
	* @parameter: bool _selected: If the sliders are being checked for selection
	* @return: void
	********************/
	void SelectSliders(bool _selected);

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
	bool m_mouseDown;
	v2float m_mousePos;
	v2float m_mousePosActual;
	bool m_initialRayCast;

	// Renderer Variables
	DX10_Renderer* m_pDX10_Renderer;

	// Camera
	DX10_Camera_FirstPerson* m_pCamera;

	// Shaders
	DX10_Shader_LitTex* m_pShader_LitTex;
	DX10_Shader_Sprite* m_pShader_Sprite;

	// Meshes
	DX10_Mesh* m_pMesh_Floor;
	DX10_Mesh* m_pMesh_Sphere;
	DX10_Mesh* m_pMesh_Capsule;
	DX10_Mesh* m_pMesh_Pyramid;

	// Sprites
	DXSprite* m_pSprite_InstructionsLeft;
	DXSprite* m_pSprite_InstructionsRight;

	// Sliders
	//TSliderBar* m_pSlider_WindSpeed;
	//TSliderBar* m_pSlider_ClothWidth;
	//TSliderBar* m_pSlider_ClothHeight;
	//TSliderBar* m_pSlider_HookAmount;
	//TSliderBar* m_pSlider_BlastRadius;

	// Objects
	DX10_Obj_LitTex* m_pObj_Floor;
	DX10_Obj_LitTex* m_pObj_Sphere;
	DX10_Obj_LitTex* m_pObj_Capsule;
	DX10_Obj_LitTex* m_pObj_Pyramid;

	// Shadowing Variables
	DX10_Shader_ShadowMap* m_pShader_ShadowMap;
	DX10_Shader_Shadow* m_pShader_Shadow;
	DX10_Shader_ShadowSoft* m_pShader_ShadowSoft;
	DX10_ShadowMap* m_pShadowMap[2];

	// TO DO CAL
	DX10_Shader_Blur* m_pShader_Blur;
};

#endif // __APPLICATION_H__


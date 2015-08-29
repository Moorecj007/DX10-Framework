/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Renderer.h
* Description : A renderer using GDI for 2D graphics
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __GDI_RENDERER_H__
#define __GDI_RENDERER_H__

// Library Includes
#include <vector>

// Local Includes
#include "GDI_Backbuffer.h"

class CGDI_Renderer
{
public:

	/***********************
	* CGDI_Renderer: Default Constructor for GDI Renderer class
	* @author: Callan Moore
	********************/
	CGDI_Renderer();

	/***********************
	* ~CGDI_Renderer: Default Destructor for GDI Renderer class
	* @author: Callan Moore
	********************/
	~CGDI_Renderer();

	/***********************
	* Initialise: Initialise the GDI Renderer for use
	* @author: Callan Moore
	* @parameter: _hWnd: Handle to the client window
	* @parameter: _hInstance: Handle to the client Instance
	* @parameter: _clientWidth: Width of the client window
	* @parameter: _clientHeight: Height of the client window
	* @return: bool: Successful or not
	********************/
	bool Initialise(HWND _hWnd, HINSTANCE _hInstance, int _clientWidth, int _clientHeight);
	
	/***********************
	* Render: Render a given "Object"
	* @author: Callan Moore
	* @parameter: _pPoints: Point list in clockwise order
	* @parameter: _color: color to fill the object with
	* @parameter: _size: How many points in the object
	* @return: void
	********************/
	void Render(POINT* _pPoints, COLORREF _color, int _size);
	
	/***********************
	* BeginRender: Clear and prepare the Backbuffer for input
	* @author: Callan Moore
	* @return: void
	********************/
	void BeginRender();
	
	/***********************
	* EndRender: Stop the Backbuffer from taking input and present to the screen
	* @author: Callan Moore
	* @return: void
	********************/
	void EndRender();

private:

	// Client Variables
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	int m_clientWidth;
	int m_clientHeight;

	// GDI Renderering Variables
	CBackBuffer* m_pBackBuffer;
};

#endif // __GDI_RENDERER_H__


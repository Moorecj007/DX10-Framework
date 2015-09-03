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

class GDI_Renderer
{
public:

	/***********************
	* CGDI_Renderer: Default Constructor for GDI Renderer class
	* @author: Callan Moore
	********************/
	GDI_Renderer();

	/***********************
	* ~CGDI_Renderer: Default Destructor for GDI Renderer class
	* @author: Callan Moore
	********************/
	~GDI_Renderer();

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
	* Render: Render a given polygon
	* @author: Callan Moore
	* @parameter: _pPoints: Point list in clockwise order
	* @parameter: _color: color to fill the polygon with
	* @parameter: _size: How many points in the polygon
	* @return: void
	********************/
	void RenderPolygon(v2float* _pPoints, COLORREF _color, int _size);

	/***********************
	* RenderEllipse: Render a given ellipse
	* @author: Callan Moore
	* @parameter: _center: position of the Center of the Ellipse
	* @parameter: _color: Color to fill the Ellipse with
	* @parameter: _radius: Radius of the Ellipse
	* @return: void
	********************/
	void RenderEllipse(v2float _center, COLORREF _color, float _radius);

	/***********************
	* RenderLine: Render a Line
	* @author: Callan Moore
	* @parameter: v2float _posA: Starting position
	* @parameter: v2float _posB: Ending position
	* @parameter: COLORREF _color: Color for the line
	* @return: void
	********************/
	void RenderLine(v2float _posA, v2float _posB, COLORREF _color);
	
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


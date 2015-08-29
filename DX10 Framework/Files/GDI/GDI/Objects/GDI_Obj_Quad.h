/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Obj_Quad.h
* Description : A Quadrilateral for GDI use
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __GDI_OBJ_QUAD_H__
#define __GDI_OBJ_QUAD_H__

// Local Includes
#include "../GDI_Renderer.h"

// Library Includes
#include <vector>

class CGDI_Quad
{
public:

	/***********************
	* CGDI_Quad: Default Constructor for GDI Quad class
	* @author: Callan Moore
	********************/
	CGDI_Quad();

	/***********************
	* CGDI_Quad: Default Constructor for GDI Quad class
	* @author: Callan Moore
	********************/
	~CGDI_Quad();

	/***********************
	* Initialise: Initialise the Quad for use
	* @author: Callan Moore
	* @parameter: _pGDIRenderer: GDI renderer for this object
	* @parameter: _pos: Initial position of the Quad (Center point)
	* @parameter: _horizontalSize: Horizontal size of the Quad
	* @parameter: _verticalSize: Vertical size of the Quad
	* @parameter: _color: Color of the Quad
	* @return: bool: Successful or not
	********************/
	bool Initialise(CGDI_Renderer* _pGDIRenderer, v2float _pos, float _horizontalSize, float _verticalSize, COLORREF _color);
	
	/***********************
	* Render: Render the Quad
	* @author: Callan Moore
	* @return: void
	********************/
	void Render();

private:

	CGDI_Renderer* m_pGDIRenderer;

	v2float m_pos;
	POINT* m_points;
	COLORREF m_color;
	int m_size;

	// Physics

};

#endif	// __GDI_OBJ_QUAD_H__

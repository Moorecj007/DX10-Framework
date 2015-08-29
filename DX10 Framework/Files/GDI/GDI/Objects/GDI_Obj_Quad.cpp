/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Obj_Quad.cpp
* Description : A Quadrilateral for GDI use
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "GDI_Obj_Quad.h"

CGDI_Quad::CGDI_Quad()
{
}

CGDI_Quad::~CGDI_Quad()
{
	// delete the points array to free memory
	delete[] m_points;
	m_points = 0;
}

bool CGDI_Quad::Initialise(CGDI_Renderer* _pGDIRenderer, v2float _pos, float _horizontalSize, float _verticalSize, COLORREF _color)
{
	// assign Member variables
	m_pGDIRenderer = _pGDIRenderer;
	m_pos = _pos;
	m_color = _color;
	m_size = 4;
	
	// Create a new array of points
	m_points = new POINT[4];

	// Top Left point
	m_points[0].x = (LONG)(_pos.x - (_horizontalSize / 2.0f));
	m_points[0].y = (LONG)(_pos.y - (_verticalSize / 2.0f));

	// Top Right point
	m_points[1].x = (LONG)(_pos.x + (_horizontalSize / 2.0f));
	m_points[1].y = (LONG)(_pos.y - (_verticalSize / 2.0f));

	// Bottom Right point
	m_points[2].x = (LONG)(_pos.x + (_horizontalSize / 2.0f));
	m_points[2].y = (LONG)(_pos.y + (_verticalSize / 2.0f));

	// Bottom Left point
	m_points[3].x = (LONG)(_pos.x - (_horizontalSize / 2.0f));
	m_points[3].y = (LONG)(_pos.y + (_verticalSize / 2.0f));

	return true;
}

void CGDI_Quad::Render()
{
	m_pGDIRenderer->Render(m_points, m_color, m_size);

}
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

GDI_Obj_Quad::GDI_Obj_Quad(GDI_Renderer* _pGDIRenderer)
{
	m_pGDIRenderer = _pGDIRenderer;
}

GDI_Obj_Quad::~GDI_Obj_Quad()
{
	
}

bool GDI_Obj_Quad::Initialise(Physics_Body_2D* _pPhysicsBody, COLORREF _color)
{
	// Assign Member variables
	m_pPhysicsBody = _pPhysicsBody;
	m_color = _color;
	m_size = 4;

	// Create a new array of points
	m_points = new v2float[m_size];

	return true;
}

void GDI_Obj_Quad::Process(float _dt)
{
	v2float pos = m_pPhysicsBody->GetPosition();
	v2float scale = m_pPhysicsBody->GetScale();
	float angle = m_pPhysicsBody->GetAngle();

	// Top Left point
	m_points[0].x = (LONG)(pos.x - (scale.x / 2.0f));
	m_points[0].y = (LONG)(pos.y - (scale.y / 2.0f));

	// Top Right point
	m_points[1].x = (LONG)(pos.x + (scale.x / 2.0f));
	m_points[1].y = (LONG)(pos.y - (scale.y / 2.0f));

	// Bottom Right point
	m_points[2].x = (LONG)(pos.x + (scale.x / 2.0f));
	m_points[2].y = (LONG)(pos.y + (scale.y / 2.0f));

	// Bottom Left point
	m_points[3].x = (LONG)(pos.x - (scale.x / 2.0f));
	m_points[3].y = (LONG)(pos.y + (scale.y / 2.0f));

	// Calculate the Rotation
	for (int i = 0; i < m_size; i++)
	{
		CalcRotation(&m_points[i], angle, pos);
	}
}

/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Obj_Polygon.cpp
* Description : A Polygon for GDI use
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "GDI_Obj_Polygon.h"

GDI_Obj_Polygon::GDI_Obj_Polygon(GDI_Renderer* _pGDIRenderer)
{
	m_pGDIRenderer = _pGDIRenderer;
}

GDI_Obj_Polygon::~GDI_Obj_Polygon()
{
}

bool GDI_Obj_Polygon::Initialise(Physics_Body_2D* _pPhysicsBody, COLORREF _colorFill, COLORREF _colorOutline)
{
	if (_pPhysicsBody == 0)
	{
		// Initialise fails if their is no body
		return false;
	}

	// Assign Member variables
	m_pPhysicsBody = _pPhysicsBody;
	m_colorFill = _colorFill;
	m_colorOutline = _colorOutline;
	m_size = m_pPhysicsBody->GetSize();

	// Create a new array of points
	m_points = new v2float[m_size];

	return true;
}

void GDI_Obj_Polygon::Process(float _dt)
{
	// Get Shape information from the Physics Body
	v2float pos = m_pPhysicsBody->GetPosition();
	UINT size = m_pPhysicsBody->GetSize();
	float angle = m_pPhysicsBody->GetAngle();

	// Create a local array of the polygons points
	for (UINT i = 0; i < m_pPhysicsBody->GetSize(); i++)
	{
		m_points[i] = m_pPhysicsBody->GetPoints()[i];
	}

	// Calculate the Rotation
	for (int i = 0; i < m_size; i++)
	{
		m_points[i] = m_points[i] + pos;
		CalcRotation(&m_points[i], angle, pos);
	}
}

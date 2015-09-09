/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Obj_Circle.h
* Description : A Circle for GDI use
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "GDI_Obj_Circle.h"

GDI_Obj_Circle::GDI_Obj_Circle(GDI_Renderer* _pGDIRenderer)
{
	m_pGDIRenderer = _pGDIRenderer;
}

GDI_Obj_Circle::~GDI_Obj_Circle()
{
	ReleasePtrArray(m_points);
}

bool GDI_Obj_Circle::Initialise(Physics_Body_2D* _pPhysicsBody, COLORREF _colorFill, COLORREF _colorOutline)
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

	// Create a new array of points
	m_points = 0;

	return true;
}

void GDI_Obj_Circle::Process(float _dt)
{

}

void GDI_Obj_Circle::Render()
{
	m_pGDIRenderer->RenderEllipse(m_pPhysicsBody->GetPosition(), m_colorFill, m_colorOutline, m_pPhysicsBody->GetRadius());
}

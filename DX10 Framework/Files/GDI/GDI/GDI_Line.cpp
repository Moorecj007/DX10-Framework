/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Line.cpp
* Description : A line for drawing in GDI
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "GDI_Line.h"

GDI_Line::GDI_Line(GDI_Renderer* _pGDI_Renderer)
{
	m_pGDI_Renderer = _pGDI_Renderer;
	m_pJoint = 0;
}

GDI_Line::~GDI_Line()
{
	ReleasePtr(m_pJoint);
}

bool GDI_Line::Initialise(Physics_Rope_2D* _pJoint, COLORREF _color)
{
	// Check if the Joint is valid
	if (_pJoint == 0)
	{
		return false;
	}

	m_pJoint = _pJoint;
	m_color = _color;

	return true;
}

bool GDI_Line::Initialise(v2float _posA, v2float _posB, COLORREF _color)
{
	m_posA = _posA;
	m_posB = _posB;
	m_color = _color;

	return true;
}

void GDI_Line::Process(float _dt)
{
	if (m_pJoint != 0)
	{
		m_posA = m_pJoint->GetAnchorA();
		m_posB = m_pJoint->GetAnchorB();
	}
}

void GDI_Line::Render()
{
	m_pGDI_Renderer->RenderLine(m_posA, m_posB, m_color);
}
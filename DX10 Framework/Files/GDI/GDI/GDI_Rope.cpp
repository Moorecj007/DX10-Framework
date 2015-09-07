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
#include "GDI_Rope.h"

GDI_Rope::GDI_Rope(GDI_Renderer* _pGDI_Renderer)
{
	m_pGDI_Renderer = _pGDI_Renderer;
	m_pPhysRope = 0;
}

GDI_Rope::~GDI_Rope()
{
	ReleasePtr(m_pPhysRope);
}

bool GDI_Rope::Initialise(Physics_Rope_2D* _pPhysRope, COLORREF _color)
{
	// Check if the Joint is valid
	if (_pPhysRope == 0)
	{
		return false;
	}

	m_pPhysRope = _pPhysRope;
	m_color = _color;

	return true;
}

bool GDI_Rope::Initialise(v2float _posA, v2float _posB, COLORREF _color)
{
	m_posA = _posA;
	m_posB = _posB;
	m_color = _color;

	return true;
}

void GDI_Rope::Process(float _dt)
{
	if (m_pPhysRope != 0)
	{
		m_posA = m_pPhysRope->GetAnchorA();
		m_posB = m_pPhysRope->GetAnchorB();
	}
}

void GDI_Rope::Render()
{
	m_pGDI_Renderer->RenderLine(m_posA, m_posB, m_color);
}
/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Spring.cpp
* Description : GDI operations for a Spring
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "GDI_Spring.h"

GDI_Spring::GDI_Spring(GDI_Renderer* _pGDI_Renderer)
{
	m_pGDI_Renderer = _pGDI_Renderer;
}

GDI_Spring::~GDI_Spring()
{
	ReleasePtr(m_pSpring);
}

bool GDI_Spring::Initialise(Physics_Spring_2D* _pSpring, COLORREF _color)
{
	if (_pSpring == 0)
	{
		// Spring pointer is invalid
		return false;
	}
	m_pSpring = _pSpring;
	m_color = _color;

	return true;
}

void GDI_Spring::Process(float _dt)
{

}

void GDI_Spring::Render()
{

}

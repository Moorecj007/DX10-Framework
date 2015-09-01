/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Obj_Generic.cpp
* Description : A Generic Object for GDI use (Abstract)
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "GDI_Obj_Generic.h"

GDI_Obj_Generic::GDI_Obj_Generic()
{
}


GDI_Obj_Generic::~GDI_Obj_Generic()
{
	ReleasePtr(m_pPhysicsBody);
	ReleasePtrArray(m_points);
}

void GDI_Obj_Generic::CalcRotation(v2float* _point, float _angle, v2float _pos)
{
	// Translate the point to origin
	(*_point).x = (*_point).x - (LONG)_pos.x;
	(*_point).y = (*_point).y - (LONG)_pos.y;

	v2float pt;
	pt.x = (LONG)((*_point).x * cos(_angle) - (*_point).y * sin(_angle));
	pt.y = (LONG)((*_point).y * cos(_angle) + (*_point).x * sin(_angle));

	(*_point).x = pt.x + (LONG)_pos.x;
	(*_point).y = pt.y + (LONG)_pos.y;
}

void GDI_Obj_Generic::Render()
{
	m_pGDIRenderer->RenderPolygon(m_points, m_color, m_size);
}
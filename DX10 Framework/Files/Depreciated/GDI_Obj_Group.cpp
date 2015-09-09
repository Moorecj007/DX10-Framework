/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Obj_Group.cpp
* Description : A group ob objects that act as one
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "GDI_Obj_Group.h"

GDI_Obj_Group::GDI_Obj_Group()
{
	m_pObjects = new std::vector<GDI_Obj_Generic*>;
}

GDI_Obj_Group::~GDI_Obj_Group()
{
}

bool GDI_Obj_Group::Initialise(GDI_Renderer* _pGDI_Renderer, std::vector<Physics_Body_2D*>* _pPhysBodies, COLORREF _colorFill, COLORREF _colorOutline)
{

	for (UINT i = 0; i < _pPhysBodies->size(); i++)
	{
		GDI_Obj_Generic* pTempPoly = new GDI_Obj_Polygon(_pGDI_Renderer);
		VALIDATE(pTempPoly->Initialise((*_pPhysBodies)[i], _colorFill, _colorOutline));
		m_pObjects->push_back(pTempPoly);
	}

	return true;
}

void GDI_Obj_Group::Process(float _dt)
{
	for (UINT i = 0; i < m_pObjects->size(); i++)
	{
		(*m_pObjects)[i]->Process(_dt);
	}
}

void GDI_Obj_Group::Render()
{
	for (UINT i = 0; i < m_pObjects->size(); i++)
	{
		(*m_pObjects)[i]->Render();
	}
}

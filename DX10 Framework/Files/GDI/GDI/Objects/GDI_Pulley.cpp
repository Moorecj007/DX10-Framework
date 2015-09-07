/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : GDI_Pulley.h
* Description : GDI operations for a pulley system
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "GDI_Pulley.h"

GDI_Pulley::GDI_Pulley(GDI_Renderer* _pGDI_Renderer, Physics_World_2D* _pPhysWorld)
{
	m_pGDI_Renderer = _pGDI_Renderer;
	m_pPhysWorld = _pPhysWorld;
}

GDI_Pulley::~GDI_Pulley()
{
	ReleasePtr(m_pStopperA);
	ReleasePtr(m_pStopperB);
	ReleasePtr(m_pPulley);	
}

bool GDI_Pulley::Initialise(Physics_Pulley_2D* _pJoint, COLORREF _color)
{
	if (_pJoint == 0)
	{
		return false;
	}
	m_pPulley = _pJoint;
	m_color = _color;

	v2float groundAnchorA = m_pPulley->GetGroundAnchorA();
	v2float groundAnchorB = m_pPulley->GetGroundAnchorB();

	Physics_Body_2D* pTempBody;
	TPhysicsProperties physProps;
	v2float* pPoints;

	m_pStopperA = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -5.0f, -5.0f };
	pPoints[1] = { 5.0f, -5.0f };
	pPoints[2] = { 5.0f, 5.0f };
	pPoints[3] = { -5.0f, 5.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = groundAnchorA;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pStopperA->Initialise(pTempBody, _color, _color));

	m_pStopperB = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -5.0f, -5.0f };
	pPoints[1] = { 5.0f, -5.0f };
	pPoints[2] = { 5.0f, 5.0f };
	pPoints[3] = { -5.0f, 5.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = groundAnchorB;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pStopperB->Initialise(pTempBody, _color, _color));

	return true;
}

void GDI_Pulley::Process(float _dt)
{
	m_pStopperA->Process(_dt);
	m_pStopperB->Process(_dt);
}

void GDI_Pulley::Render()
{
	v2float anchorA = m_pPulley->GetAnchorA();
	v2float anchorB = m_pPulley->GetAnchorB();
	v2float groundAnchorA = m_pPulley->GetGroundAnchorA();
	v2float groundAnchorB = m_pPulley->GetGroundAnchorB();

	m_pGDI_Renderer->RenderLine(anchorA, groundAnchorA, m_color);
	m_pGDI_Renderer->RenderLine(groundAnchorA, groundAnchorB, m_color);
	m_pGDI_Renderer->RenderLine(groundAnchorB, anchorB, m_color);

	m_pStopperA->Render();
	m_pStopperB->Render();
}
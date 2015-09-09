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
#include "GDI_Obj_Pulley.h"

GDI_Obj_Pulley::GDI_Obj_Pulley(GDI_Renderer* _pGDI_Renderer, Physics_World_2D* _pPhysWorld)
{
	m_pGDI_Renderer = _pGDI_Renderer;
	m_pPhysWorld = _pPhysWorld;
}

GDI_Obj_Pulley::~GDI_Obj_Pulley()
{
	ReleasePtr(m_pStopperA);
	ReleasePtr(m_pStopperB);
	ReleasePtr(m_pPulley);	
}

bool GDI_Obj_Pulley::Initialise(Physics_Pulley_2D* pPhysPulley, COLORREF _color)
{
	if (pPhysPulley == 0)
	{
		// Initialise fails if their is no Pulley Joint
		return false;
	}

	// Assign member variables
	m_pPulley = pPhysPulley;
	m_color = _color;


	// Create two small static objects at the ground anchor points
	v2float groundAnchorA = m_pPulley->GetGroundAnchorA();
	v2float groundAnchorB = m_pPulley->GetGroundAnchorB();

	Physics_Body_2D* pTempBody;
	TPhysicsProperties physProps;
	v2float* pPoints;

	// Static stopper for the first ground anchor above the first pulley object
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


	// Static stopper for the Second ground anchor above the second pulley object
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

void GDI_Obj_Pulley::Process(float _dt)
{
	m_pStopperA->Process(_dt);
	m_pStopperB->Process(_dt);
}

void GDI_Obj_Pulley::Render()
{
	v2float anchorA = m_pPulley->GetAnchorA();
	v2float anchorB = m_pPulley->GetAnchorB();
	v2float groundAnchorA = m_pPulley->GetGroundAnchorA();
	v2float groundAnchorB = m_pPulley->GetGroundAnchorB();

	// Render two lines to show the connection between the pulley object and the corresponding ground anchor
	m_pGDI_Renderer->RenderLine(anchorA, groundAnchorA, m_color);
	m_pGDI_Renderer->RenderLine(groundAnchorB, anchorB, m_color);

	// Render both static stopper objects
	m_pStopperA->Render();
	m_pStopperB->Render();
}
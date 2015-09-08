/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Level_03.h
* Description : Builds and runs Level 03 for the game
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "Level_03.h"

Level_03::Level_03(GDI_Renderer* _pGDI_Renderer, int _clientWidth, int _clientHeight)
{
	m_pGDI_Renderer = _pGDI_Renderer;
	m_clientWidth = _clientWidth;
	m_clientHeight = _clientHeight;
}

Level_03::~Level_03()
{
}

bool Level_03::ContructLevel()
{
	TPhysicsProperties physProps;
	v2float* pPoints;
	Physics_Body_2D* pTempBody;
	GDI_Obj_Generic* pTempObject;
	Physics_Rope_2D* pPhysRope;
	GDI_Rope* pRope;

	VALIDATE(InitialSetup());

	/*
		The Gem
	*/

	m_pGem = new GDI_Obj_Circle(m_pGDI_Renderer);
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = true;
	physProps.dampening = 15.0f;
	physProps.radius = 15.0f;
	physProps.density = 1.0f;
	physProps.pos = { 125, 935 };
	physProps.collisionType = CT_GEM;
	physProps.collideWith = (CT_STANDARD | CT_BREAKABLE | CT_WINZONE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pGem->Initialise(pTempBody, colorRef::YELLOW, colorRef::BLACK));

	/*
		Win Zone
	*/

	m_pWinZone = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -30.0f, -30.0f };
	pPoints[1] = {  30.0f, -30.0f };
	pPoints[2] = {  30.0f,  30.0f };
	pPoints[3] = { -30.0f,  30.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 940, 920 };
	physProps.collisionType = CT_WINZONE;
	physProps.collideWith = (CT_GEM);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pWinZone->Initialise(pTempBody, colorRef::GREEN, colorRef::GREEN));

	/*
		Static level design
	*/


	/*
		Disco Octagon
	*/
	
	GDI_Obj_Generic* pDiscoOct = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[8];
	pPoints[0] = {  0.0f,  -60.0f };
	pPoints[1] = { 40.0f, -40.0f };
	pPoints[2] = { 60.0f, 0.0f };
	pPoints[3] = { 40.0f, 40.0f };
	pPoints[4] = { 0.0f, 60.0f };
	pPoints[5] = { -40.0f, 40.0f };
	pPoints[6] = { -60.0f, 0.0f };
	pPoints[7] = { -40.0f, -40.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 8;
	physProps.density = 2.0f;
	physProps.friction = 40;
	physProps.pos = { 500, 200 };
	physProps.collisionType = CT_BREAKABLE;
	physProps.collideWith = (CT_GEM | CT_STANDARD | CT_ENEMY | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pDiscoOct->Initialise(pTempBody, colorRef::BLUE, colorRef::BLACK));
	m_pObjBreakables->push_back(pDiscoOct);

	m_pPhysWorld->CreateRevoluteJoint(m_pBackground->GetPhysicsBody(), pDiscoOct->GetPhysicsBody(), { 500, 140 }, false);

	//pPhysRope = m_pPhysWorld->CreateRopeJoint(pDiscoOct->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 0.0f, -60.0f }, { 500.0f, 0.0f }, false);
	//pRope = new GDI_Rope(m_pGDI_Renderer);
	//pRope->Initialise(pPhysRope, colorRef::YELLOW);
	//m_pObjRopes->push_back(pRope);

	/*
		Spring
	*/

	GDI_Obj_Generic* pSpringPad = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -10.0f, -50.0f };
	pPoints[1] = {  10.0f, -50.0f };
	pPoints[2] = {  10.0f,  50.0f };
	pPoints[3] = { -10.0f,  50.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.density = 1.0f;
	physProps.pos = { 95, 893 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_GEM | CT_STANDARD | CT_ENEMY | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pSpringPad->Initialise(pTempBody, colorRef::BLUE, colorRef::BLACK));
	m_pObjDynamics->push_back(pSpringPad);

	Physics_Spring_2D* pPhysSpring =  m_pPhysWorld->CreateSpring(pSpringPad->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 0, 0 }, { 0, 893 }, 25.0f, 0.1f);
	ReleasePtr(pPhysSpring);

	/*
		Spring Ram
	*/

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -50.0f, -15.0f };
	pPoints[1] = {  50.0f, -15.0f };
	pPoints[2] = {  50.0f,  15.0f };
	pPoints[3] = { -50.0f,  15.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.density = 400.0f;
	physProps.friction = 0.3f;
	physProps.pos = { 100, 150 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_GEM | CT_STANDARD | CT_ENEMY | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLUE, colorRef::BLACK));
	m_pObjDynamics->push_back(pTempObject);

	pPhysRope = m_pPhysWorld->CreateRopeJoint(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { -45.0f, -0.0f }, { 205.0f, 0 }, false);
	pRope = new GDI_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::YELLOW);
	m_pObjRopes->push_back(pRope);

	pPhysRope = m_pPhysWorld->CreateRopeJoint(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 45.0f, 0.0f }, { 295.0f, 0 }, false);
	pRope = new GDI_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::BLACK);
	m_pObjRopes->push_back(pRope);



	return true;
}
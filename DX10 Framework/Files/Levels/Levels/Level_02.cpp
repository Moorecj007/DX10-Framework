/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Level_02.h
* Description : Builds and runs Level 02 for the game
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "Level_02.h"

Level_02::Level_02(GDI_Renderer* _pGDI_Renderer, int _clientWidth, int _clientHeight)
{
	m_pGDI_Renderer = _pGDI_Renderer;
	m_clientWidth = _clientWidth;
	m_clientHeight = _clientHeight;
}

Level_02::~Level_02()
{
}

bool Level_02::ContructLevel()
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
	physProps.pos = { 950, 50 };
	physProps.collisionType = CT_GEM;
	physProps.collideWith = (CT_STANDARD | CT_BREAKABLE | CT_WINZONE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pGem->Initialise(pTempBody, colorRef::YELLOW, colorRef::BLACK));

	pPhysRope = m_pPhysWorld->CreateRopeJoint(m_pGem->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 0.0f, 0.0f }, { 950, 0.0f }, false);
	pRope = new GDI_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::YELLOW);
	m_pObjRopes->push_back(pRope);

	/*
		Win Zone
	*/

	m_pWinZone = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -15.0f, -20.0f };
	pPoints[1] = {  15.0f, -20.0f };
	pPoints[2] = {  15.0f,  20.0f };
	pPoints[3] = { -15.0f,  20.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 25, 850 };
	physProps.collisionType = CT_WINZONE;
	physProps.collideWith = (CT_GEM);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pWinZone->Initialise(pTempBody, colorRef::GREEN, colorRef::GREEN));

	/*
		Static level design
	*/

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -75.0f, -75.0f };
	pPoints[1] = { 75.0f, -75.0f };
	pPoints[2] = { 75.0f, 75.0f };
	pPoints[3] = { -75.0f, 75.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 75, 950 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -375.0f, -75.0f };
	pPoints[1] = { 375.0f, -75.0f };
	pPoints[2] = { 375.0f, 75.0f };
	pPoints[3] = { -375.0f, 75.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 625, 950 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -200.0f, -10.0f };
	pPoints[1] = { 200.0f, -10.0f };
	pPoints[2] = { 200.0f, 10.0f };
	pPoints[3] = { -200.0f, 10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.isStatic = true;
	physProps.density = 1.0f;
	physProps.friction = 0.3f;
	physProps.pos = { 200, 600 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[3];
	pPoints[0] = { -300.0f,  200.0f };
	pPoints[1] = {  300.0f, -200.0f };
	pPoints[2] = {  300.0f,  200.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 3;
	physProps.isStatic = true;
	physProps.density = 1.0f;
	physProps.friction = 0.3f;
	physProps.pos = { 700, 700 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[3];
	pPoints[0] = { -20.0f, -40.0f };
	pPoints[1] = {  20.0f, -40.0f };
	pPoints[2] = { -20.0f,  40.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 3;
	physProps.isStatic = true;
	physProps.density = 1.0f;
	physProps.friction = 0.3f;
	physProps.restitution = 3.0f;
	physProps.pos = { 30, 150 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -250.0f, -10.0f };
	pPoints[1] = {  250.0f, -10.0f };
	pPoints[2] = {  250.0f,  10.0f };
	pPoints[3] = { -250.0f,  10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.isStatic = true;
	physProps.friction = 0.3f;
	physProps.angle = DegreesToRadians(-20.0f);
	physProps.pos = { 750, 150 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	/*
		Bridge
	*/

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -50.0f, -10.0f };
	pPoints[1] = { 50.0f, -10.0f };
	pPoints[2] = { 100.0f, 10.0f };
	pPoints[3] = { -100.0f, 10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.density = 1000.0f;
	physProps.friction = 0.3f;
	physProps.pos = { 225, 650 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLUE, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	pRope = new GDI_Rope(m_pGDI_Renderer);
	pPhysRope = m_pPhysWorld->CreateRopeJoint(m_pBackground->GetPhysicsBody(), pTempObject->GetPhysicsBody(), { 175, 600 }, { -50.0f, -10.0f }, false);
	pRope->Initialise(pPhysRope, colorRef::YELLOW);
	m_pObjRopes->push_back(pRope);

	pPhysRope = m_pPhysWorld->CreateRopeJoint(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 50.0f, -10.0f }, { 275, 600 }, false);
	pRope = new GDI_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::YELLOW);
	m_pObjRopes->push_back(pRope);

	/*
		See-Saw Lever
	*/

	GDI_Obj_Generic* pPivot = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[3];
	pPoints[0] = { -50.0f, 30.0f };
	pPoints[1] = { 0.0f, -25.0f };
	pPoints[2] = { 50.0f, 30.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 3;
	physProps.density = 1.0f;
	physProps.friction = 0.3f;
	physProps.pos = { 225, 560 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pPivot->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pPivot);

	GDI_Obj_Generic* pLever = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -200.0f, -5.0f };
	pPoints[1] = { 200.0f, -5.0f };
	pPoints[2] = { 200.0f, 5.0f };
	pPoints[3] = { -200.0f, 5.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.density = 1.0f;
	physProps.friction = 0.3f;
	physProps.pos = { 225, 535 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pLever->Initialise(pTempBody, colorRef::BLUE, colorRef::BLACK));
	m_pObjStatics->push_back(pLever);

	m_pPhysWorld->CreateRevoluteJoint(pLever->GetPhysicsBody(), pPivot->GetPhysicsBody(), { 225, 535 }, false);

	pTempObject = new GDI_Obj_Circle(m_pGDI_Renderer);
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = true;
	physProps.dampening = 15.0f;
	physProps.radius = 30.0f;
	physProps.pos = { 400, 200 };
	physProps.density = 300.0f;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_WINZONE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLUE, colorRef::BLACK));
	m_pObjDynamics->push_back(pTempObject);

	pPhysRope = m_pPhysWorld->CreateRopeJoint(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 0.0f, 0.0f }, { 400, 0.0f }, false);
	pRope = new GDI_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::YELLOW);
	m_pObjRopes->push_back(pRope);
	


	return true;
}
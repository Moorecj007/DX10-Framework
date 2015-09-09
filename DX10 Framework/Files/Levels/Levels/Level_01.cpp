/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Level_01.h
* Description : Builds and runs Level 01 for the game
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "Level_01.h"

Level_01::Level_01(GDI_Renderer* _pGDI_Renderer, int _clientWidth, int _clientHeight)
{
	// Assign member variables
	m_pGDI_Renderer = _pGDI_Renderer;
	m_clientWidth = _clientWidth;
	m_clientHeight = _clientHeight;
}

Level_01::~Level_01()
{
}

bool Level_01::ContructLevel()
{
	// Create Temporary points for use in object creation
	TPhysicsProperties physProps;
	v2float* pPoints;
	Physics_Body_2D* pTempBody;
	GDI_Obj_Generic* pTempObject;
	Physics_Rope_2D* pPhysRope;
	GDI_Obj_Rope* pRope;

	VALIDATE(InitialSetup());

	/*
	The Gem
	*/

	m_pGem = new GDI_Obj_Circle(m_pGDI_Renderer);
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = true;
	physProps.damping = 1.0f;
	physProps.radius = 15.0f;
	physProps.density = 1.0f;
	physProps.friction = 0.1f;
	physProps.pos = { 750, 50 };
	physProps.collisionType = CT_GEM;
	physProps.collideWith = (CT_STANDARD | CT_BREAKABLE | CT_WINZONE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pGem->Initialise(pTempBody, colorRef::YELLOW, colorRef::BLACK));

	pPhysRope = m_pPhysWorld->CreateRope(m_pGem->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 0.0f, 0.0f }, { 750, 0.0f }, false);
	pRope = new GDI_Obj_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::PURPLE);
	m_pRopes_Cuttable->push_back(pRope);

	/*
	Win Zone
	*/

	m_pWinZone = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -15.0f, -20.0f };
	pPoints[1] = { 15.0f, -20.0f };
	pPoints[2] = { 15.0f, 20.0f };
	pPoints[3] = { -15.0f, 20.0f };
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
	physProps.friction = 0.3f;
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
	physProps.friction = 0.3f;
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
	pPoints[0] = { -300.0f, 200.0f };
	pPoints[1] = { 150.0f, -200.0f };
	pPoints[2] = { 150.0f, 200.0f };
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
	pPoints[1] = { 20.0f, -40.0f };
	pPoints[2] = { -20.0f, 40.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 3;
	physProps.isStatic = true;
	physProps.density = 1.0f;
	physProps.friction = 0.3f;
	physProps.restitution = 1.0f;
	physProps.pos = { 30, 150 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -100.0f, -10.0f };
	pPoints[1] = { 250.0f, -10.0f };
	pPoints[2] = { 250.0f, 10.0f };
	pPoints[3] = { -100.0f, 10.0f };
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

	GDI_Obj_Generic* pCartStopper = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -15.0f, -5.0f };
	pPoints[1] = { 10.0f, -5.0f };
	pPoints[2] = { 10.0f, 5.0f };
	pPoints[3] = { -15.0f, 5.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.isStatic = true;
	physProps.friction = 0.3f;
	physProps.angle = DegreesToRadians(70.0f);
	physProps.pos = { 515, 220 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pCartStopper->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pCartStopper);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -100.0f, -10.0f };
	pPoints[1] = { 160.0f, -10.0f };
	pPoints[2] = { 160.0f, 10.0f };
	pPoints[3] = { -100.0f, 10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.isStatic = true;
	physProps.friction = 0.3f;
	physProps.angle = DegreesToRadians(20.0f);
	physProps.pos = { 740, 303 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	// Hidden point physics body 
	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -1.0f, -1.0f };
	pPoints[1] = { 1.0f, -1.0f };
	pPoints[2] = { 1.0f, 1.0f };
	pPoints[3] = { -1.0f, 1.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.isStatic = true;
	physProps.friction = 0.3f;
	physProps.angle = DegreesToRadians(20.0f);
	physProps.pos = { 620, 273 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::WHITE, colorRef::WHITE));
	m_pObjStatics->push_back(pTempObject);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -30.0f, -300.0f };
	pPoints[1] = { 30.0f, -300.0f };
	pPoints[2] = { 30.0f, 300.0f };
	pPoints[3] = { -30.0f, 300.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.isStatic = true;
	physProps.friction = 0.3f;
	physProps.pos = { 875, 648 };
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
	physProps.density = 50.0f;
	physProps.friction = 0.3f;
	physProps.pos = { 225, 650 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::CYAN, colorRef::BLACK));
	m_pObjDynamics->push_back(pTempObject);

	pRope = new GDI_Obj_Rope(m_pGDI_Renderer);
	pPhysRope = m_pPhysWorld->CreateRope(m_pBackground->GetPhysicsBody(), pTempObject->GetPhysicsBody(), { 175, 600 }, { -50.0f, -10.0f }, false);
	pRope->Initialise(pPhysRope, colorRef::PURPLE);
	m_pRopes_Cuttable->push_back(pRope);

	pPhysRope = m_pPhysWorld->CreateRope(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 50.0f, -10.0f }, { 275, 600 }, false);
	pRope = new GDI_Obj_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::PURPLE);
	m_pRopes_Cuttable->push_back(pRope);

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
	VALIDATE(pLever->Initialise(pTempBody, colorRef::CYAN, colorRef::BLACK));
	m_pObjDynamics->push_back(pLever);

	m_pPhysWorld->CreateRevoluteJoint(pLever->GetPhysicsBody(), pPivot->GetPhysicsBody(), { 225, 535 }, false);

	/*
	Lever Boulder
	*/

	pTempObject = new GDI_Obj_Circle(m_pGDI_Renderer);
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = true;
	physProps.damping = 0.5f;
	physProps.radius = 30.0f;
	physProps.pos = { 400, 100 };
	physProps.density = 20.0f;
	physProps.friction = 0.1f;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_WINZONE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::CYAN, colorRef::BLACK));
	m_pObjDynamics->push_back(pTempObject);

	pPhysRope = m_pPhysWorld->CreateRope(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 0.0f, 0.0f }, { 400, 0.0f }, false);
	pRope = new GDI_Obj_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::PURPLE);
	m_pRopes_Cuttable->push_back(pRope);

	/*
		Cart
	*/

	GDI_Obj_Generic* pCartBody = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -30.0f, -10.0f };
	pPoints[1] = { 30.0f, -10.0f };
	pPoints[2] = { 30.0f, 10.0f };
	pPoints[3] = { -30.0f, 10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.density = 1.0f;
	physProps.friction = 0.3f;
	physProps.pos = { 900, 55 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pCartBody->Initialise(pTempBody, colorRef::CYAN, colorRef::BLACK));
	m_pObjDynamics->push_back(pCartBody);

	GDI_Obj_Generic* pCartWheelLeft = new GDI_Obj_Circle(m_pGDI_Renderer);
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = true;
	physProps.damping = 2.0f;
	physProps.radius = 10.0f;
	physProps.pos = { 883, 65 };
	physProps.density = 1.0f;
	physProps.friction = 0.1f;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_WINZONE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pCartWheelLeft->Initialise(pTempBody, colorRef::CYAN, colorRef::BLACK));
	m_pObjDynamics->push_back(pCartWheelLeft);

	GDI_Obj_Generic* pCartWheelRight = new GDI_Obj_Circle(m_pGDI_Renderer);
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = true;
	physProps.damping = 2.0f;
	physProps.radius = 10.0f;
	physProps.pos = { 917, 65 };
	physProps.density = 1.0f;
	physProps.friction = 0.1f;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_WINZONE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pCartWheelRight->Initialise(pTempBody, colorRef::CYAN, colorRef::BLACK));
	m_pObjDynamics->push_back(pCartWheelRight);

	m_pPhysWorld->CreateRevoluteJoint(pCartWheelLeft->GetPhysicsBody(), pCartBody->GetPhysicsBody(), { 883, 65 }, false);
	m_pPhysWorld->CreateRevoluteJoint(pCartWheelRight->GetPhysicsBody(), pCartBody->GetPhysicsBody(), { 917, 65 }, false);

	/*
		Cart Trapdoor
	*/

	GDI_Obj_Generic* pCartTrapdoor = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -70.0f, -10.0f };
	pPoints[1] = { 70.0f, -10.0f };
	pPoints[2] = { 70.0f, 10.0f };
	pPoints[3] = { -70.0f, 10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.friction = 0.3f;
	physProps.density = 10.0f;
	physProps.angle = DegreesToRadians(-20.0f);
	physProps.pos = { 580, 210 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pCartTrapdoor->Initialise(pTempBody, colorRef::CYAN, colorRef::BLACK));
	m_pObjDynamics->push_back(pCartTrapdoor);

	m_pPhysWorld->CreateRevoluteJoint(pCartTrapdoor->GetPhysicsBody(), pCartStopper->GetPhysicsBody(), { 518, 225 }, false);

	pPhysRope = m_pPhysWorld->CreateRope(pCartTrapdoor->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 60.0f, 0.0f }, { 630, 0.0f }, false);
	pRope = new GDI_Obj_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::PURPLE);
	m_pRopes_Cuttable->push_back(pRope);

	/*
		Pulley System
	*/

	GDI_Obj_Generic* pPulleyRight = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -30.0f, -20.0f };
	pPoints[1] = { 30.0f, -20.0f };
	pPoints[2] = { 30.0f, 20.0f };
	pPoints[3] = { -30.0f, 20.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.friction = 0.3f;
	physProps.density = 1.0f;
	physProps.pos = { 940, 375 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pPulleyRight->Initialise(pTempBody, colorRef::CYAN, colorRef::BLACK));
	m_pObjDynamics->push_back(pPulleyRight);

	GDI_Obj_Generic* pPulleyBreakHolder = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -20.0f, -10.0f };
	pPoints[1] = { 20.0f, -10.0f };
	pPoints[2] = { 20.0f, 10.0f };
	pPoints[3] = { -20.0f, 10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.density = 3.0f;
	physProps.friction = 0.3f;
	physProps.pos = { 520, 400 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pPulleyBreakHolder->Initialise(pTempBody, colorRef::CYAN, colorRef::BLACK));
	m_pObjDynamics->push_back(pPulleyBreakHolder);

	Physics_Pulley_2D* pTempPhysPulley = m_pPhysWorld->CreatePulley(pPulleyBreakHolder->GetPhysicsBody(), pPulleyRight->GetPhysicsBody(), { 0, -5.0f }, { 0, -20.0f }, { 520, 255 }, { 940, 220 });
	GDI_Obj_Pulley* pTempPulley = new GDI_Obj_Pulley(m_pGDI_Renderer, m_pPhysWorld);
	VALIDATE(pTempPulley->Initialise(pTempPhysPulley, colorRef::GREY));
	m_pObjPulleys->push_back(pTempPulley);

	/*
		DONT BREAK object
	*/

	GDI_Obj_Generic* pDiscoOct = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[8];
	pPoints[0] = { -18.634f, -45.0f };
	pPoints[1] = { 18.634f, -45.0f };
	pPoints[2] = { 45.0f, -18.634f };
	pPoints[3] = { 45.0f, 18.634f };
	pPoints[4] = { 18.634f, 45.0f };
	pPoints[5] = { -18.634f, 45.0f };
	pPoints[6] = { -45.0f, 18.634f };
	pPoints[7] = { -45.0f, -18.634f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 8;
	physProps.density = 0.000001f;
	physProps.friction = 0.3f;
	physProps.pos = { 520, 448 };
	physProps.collisionType = CT_BREAKABLE;
	physProps.collideWith = (CT_GEM | CT_STANDARD | CT_ENEMY | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pDiscoOct->Initialise(pTempBody, colorRef::PINK, colorRef::BLACK));
	m_pObjBreakables->push_back(pDiscoOct);
	
	m_pPhysWorld->CreateRevoluteJoint(pDiscoOct->GetPhysicsBody(), pPulleyBreakHolder->GetPhysicsBody(), { 505.0f, 404.0f }, false);
	m_pPhysWorld->CreateRevoluteJoint(pDiscoOct->GetPhysicsBody(), pPulleyBreakHolder->GetPhysicsBody(), { 535.0f, 404.0f }, false);

	return true;
}
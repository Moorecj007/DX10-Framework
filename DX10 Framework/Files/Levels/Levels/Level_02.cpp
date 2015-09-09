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
	GDI_Obj_Rope* pRope;

	VALIDATE(InitialSetup());

	/*
	Create Win Zone
	*/

	m_pWinZone = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -30.0f, -20.0f };
	pPoints[1] = { 30.0f, -20.0f };
	pPoints[2] = { 30.0f, 20.0f };
	pPoints[3] = { -30.0f, 20.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 940, 930 };
	physProps.collisionType = CT_WINZONE;
	physProps.collideWith = (CT_GEM);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pWinZone->Initialise(pTempBody, colorRef::GREEN, colorRef::GREEN));

	/*
	Create the Gem
	*/

	m_pGem = new GDI_Obj_Circle(m_pGDI_Renderer);
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = true;
	physProps.radius = 15.0f;
	physProps.density = 1.0f;
	physProps.pos = { 80, 175 };
	physProps.restitution = 0.01f;
	physProps.damping = 2.0f;
	physProps.friction = 0.1f;
	physProps.collisionType = CT_GEM;
	physProps.collideWith = (CT_STANDARD | CT_BREAKABLE | CT_WINZONE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pGem->Initialise(pTempBody, colorRef::YELLOW, colorRef::BLACK));

	/*
	Create Standard Static Level Layout
	*/

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -50.0f, -10.0f };
	pPoints[1] = { 50.0f, -10.0f };
	pPoints[2] = { 50.0f, 10.0f };
	pPoints[3] = { -50.0f, 10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.friction = 0.3f;
	physProps.pos = { 925, 890 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -175.0f, -10.0f };
	pPoints[1] = { 175.0f, -10.0f };
	pPoints[2] = { 175.0f, 10.0f };
	pPoints[3] = { -175.0f, 10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.friction = 0.3f;
	physProps.pos = { 175, 200 };
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
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.friction = 0.3f;
	physProps.pos = { 840, 210 };
	physProps.angle = DegreesToRadians(-40);
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -150.0f, -10.0f };
	pPoints[1] = { 150.0f, -10.0f };
	pPoints[2] = { 150.0f, 10.0f };
	pPoints[3] = { -150.0f, 10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.friction = 0.3f;
	physProps.pos = { 150, 650 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -120.0f, -10.0f };
	pPoints[1] = { 120.0f, -10.0f };
	pPoints[2] = { 120.0f, 10.0f };
	pPoints[3] = { -120.0f, 10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.friction = 0.3f;
	physProps.pos = { 400, 750 };
	physProps.angle = DegreesToRadians(-40);
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	GDI_Obj_Generic* pHingeStatic = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -40.0f, -10.0f };
	pPoints[1] = { 50.0f, -10.0f };
	pPoints[2] = { 50.0f, 10.0f };
	pPoints[3] = { -40.0f, 10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.friction = 0.3f;
	physProps.pos = { 265, 825 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pHingeStatic->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pHingeStatic);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[3];
	pPoints[0] = { -500.0f, -500.0f };
	pPoints[1] = { 500.0f, 500.0f };
	pPoints[2] = { -500.0f, 500.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 3;
	physProps.friction = 0.3f;
	physProps.pos = { 60, 920 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -100.0f, -10.0f };
	pPoints[1] = { 100.0f, -10.0f };
	pPoints[2] = { 100.0f, 10.0f };
	pPoints[3] = { -100.0f, 10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.friction = 0.3f;
	physProps.pos = { 900, 540 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	/*
	Create the Pulley System
	*/

	GDI_Obj_Generic* pPulleySystemA = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -25.0f, -50.0f };
	pPoints[1] = { 25.0f, -50.0f };
	pPoints[2] = { 25.0f, 50.0f };
	pPoints[3] = { -25.0f, 50.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.density = 1.0f;
	physProps.size = 4;
	physProps.pos = { 250, 470 };
	physProps.collisionType = CT_ENEMY;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pPulleySystemA->Initialise(pTempBody, colorRef::RED, colorRef::BLACK));
	m_pObjDynamics->push_back(pPulleySystemA);

	GDI_Obj_Generic* pPulleySystemB = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -20.0f, -50.0f };
	pPoints[1] = { 20.0f, -50.0f };
	pPoints[2] = { 20.0f, 50.0f };
	pPoints[3] = { -20.0f, 50.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.density = 1.0f;
	physProps.size = 4;
	physProps.pos = { 840, 898 };
	physProps.collisionType = CT_ENEMY;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pPulleySystemB->Initialise(pTempBody, colorRef::RED, colorRef::BLACK));
	m_pObjDynamics->push_back(pPulleySystemB);

	Physics_Pulley_2D* pTempPhysPulley = m_pPhysWorld->CreatePulley(pPulleySystemA->GetPhysicsBody(), pPulleySystemB->GetPhysicsBody(), { 0, -50.0f }, { 0, -50.0f }, { 250, 210 }, { 840, 550 });
	GDI_Obj_Pulley* pTempPulley = new GDI_Obj_Pulley(m_pGDI_Renderer, m_pPhysWorld);
	VALIDATE(pTempPulley->Initialise(pTempPhysPulley, colorRef::GREY));
	m_pObjPulleys->push_back(pTempPulley);

	/*
	Create the Breakable objects
	*/

	GDI_Obj_Generic* pDiscoOct = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[8];
	pPoints[0] = { -24.85f, -60.0f };
	pPoints[1] = { 24.85f, -60.0f };
	pPoints[2] = { 60.0f, -24.85f };
	pPoints[3] = { 60.0f, 24.85f };
	pPoints[4] = { 24.85f, 60.0f };
	pPoints[5] = { -24.85f, 60.0f };
	pPoints[6] = { -60.0f, 24.85f };
	pPoints[7] = { -60.0f, -24.85f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 8;
	physProps.density = 0.25f;  // 0.025
	physProps.friction = 0.3f;
	//physProps.restitution = 0.6f;
	physProps.pos = { 250, 580 };
	physProps.collisionType = CT_BREAKABLE;
	physProps.collideWith = (CT_GEM | CT_STANDARD | CT_ENEMY | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pDiscoOct->Initialise(pTempBody, colorRef::PINK, colorRef::BLACK));
	m_pObjBreakables->push_back(pDiscoOct);

	/*
	Create the Cocked Gem Spring
	*/

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -10.0f, -30.0f };
	pPoints[1] = { 10.0f, -30.0f };
	pPoints[2] = { 10.0f, 30.0f };
	pPoints[3] = { -10.0f, 30.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.density = 15.0f;
	physProps.pos = { 50.0f, 160.0f };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::DARKBLUE, colorRef::BLACK));
	m_pObjDynamics->push_back(pTempObject);

	m_pPhysWorld->CreateSpring(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 0, 0 }, { 0, 160 }, 3, 0.1f, 100.0f);
	pPhysRope = m_pPhysWorld->CreateRope(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 0.0f, 0.0f }, { 0.0f, 160.0f }, false);
	pRope = new GDI_Obj_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::PURPLE);
	m_pRopes_Cuttable->push_back(pRope);

	/*
	Create the Cocked Enemy Spring
	*/

	//pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	//pPoints = new v2float[4];
	//pPoints[0] = { -10.0f, -30.0f };
	//pPoints[1] = { 10.0f, -30.0f };
	//pPoints[2] = { 10.0f, 30.0f };
	//pPoints[3] = { -10.0f, 30.0f };
	//ZeroMemory(&physProps, sizeof(physProps));
	//physProps.pPoints = pPoints;
	//physProps.size = 4;
	//physProps.density = 10.0f;
	//physProps.pos = { 930.0f, 500.0f };
	//physProps.collisionType = CT_STANDARD;
	//physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	//pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	//VALIDATE(pTempObject->Initialise(pTempBody, colorRef::DARKBLUE, colorRef::BLACK));
	//m_pObjDynamics->push_back(pTempObject);
	//
	//pPhysSpring = m_pPhysWorld->CreateSpring(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 0, 0 }, { 1000, 500 }, 3, 0.1f, 100.0f);
	//ReleasePtr(pPhysSpring);
	//
	//pPhysRope = m_pPhysWorld->CreateRopeJoint(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 0.0f, 0.0f }, { 1000.0f, 500.0f }, false);
	//pRope = new GDI_Rope(m_pGDI_Renderer);
	//pRope->Initialise(pPhysRope, colorRef::PURPLE);
	//m_pRopes_Cuttable->push_back(pRope);

	// Enemy ball in cocked spring
	pTempObject = new GDI_Obj_Circle(m_pGDI_Renderer);
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = true;
	physProps.damping = 15.0f;
	physProps.radius = 25.0f;
	physProps.density = 10.0f;
	//physProps.pos = { 890, 505 };
	physProps.pos = { 890, 320 };
	physProps.restitution = 0.0f;
	physProps.damping = 2.0f;
	physProps.friction = 0.3f;
	physProps.collisionType = CT_ENEMY;
	physProps.collideWith = (CT_STANDARD | CT_BREAKABLE | CT_GEM | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::RED, colorRef::BLACK));
	m_pObjDynamics->push_back(pTempObject);

	pPhysRope = m_pPhysWorld->CreateRope(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 0.0f, 0.0f }, { 700.0f, 320.0f }, false);
	pRope = new GDI_Obj_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::PURPLE);
	m_pRopes_Cuttable->push_back(pRope);

	pPhysRope = m_pPhysWorld->CreateRope(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 0.0f, 0.0f }, { 1000.0f, 320.0f }, false);
	pRope = new GDI_Obj_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::PURPLE);
	m_pRopes_Cuttable->push_back(pRope);

	/*
		Create Hinge joint
	*/

	GDI_Obj_Generic* pHinge = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -60.0f, -10.0f };
	pPoints[1] = { 60.0f, -10.0f };
	pPoints[2] = { 60.0f, 10.0f };
	pPoints[3] = { -60.0f, 10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.friction = 0.3f;
	physProps.pos = { 165, 825 };
	physProps.density = 500.0f;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pHinge->Initialise(pTempBody, colorRef::CYAN, colorRef::BLACK));
	m_pObjDynamics->push_back(pHinge);

	m_pPhysWorld->CreateRevoluteJoint(pHinge->GetPhysicsBody(), pHingeStatic->GetPhysicsBody(), { 220, 825 }, false);

	pPhysRope = m_pPhysWorld->CreateRope(pHinge->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { -55.0f, 0.0f }, { 110.0f, 650.0f }, false);
	pRope = new GDI_Obj_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::PURPLE);
	m_pRopes_Cuttable->push_back(pRope);

	return true;
}
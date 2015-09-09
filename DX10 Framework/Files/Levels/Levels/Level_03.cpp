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
	// Assign member variables
	m_pGDI_Renderer = _pGDI_Renderer;
	m_clientWidth = _clientWidth;
	m_clientHeight = _clientHeight;
}

Level_03::~Level_03()
{
}

bool Level_03::ContructLevel()
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
	physProps.damping = 15.0f;
	physProps.radius = 15.0f;
	physProps.density = 1.0f;
	physProps.pos = { 100, 100 };
	physProps.collisionType = CT_GEM;
	physProps.collideWith = (CT_STANDARD | CT_BREAKABLE | CT_WINZONE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pGem->Initialise(pTempBody, colorRef::YELLOW, colorRef::BLACK));

	pPhysRope = m_pPhysWorld->CreateRope(m_pGem->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 0.0f, 0.0f }, { 380.0f, 0.0f }, false);
	pRope = new GDI_Obj_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::PURPLE);
	m_pRopes_Cuttable->push_back(pRope);

	pPhysRope = m_pPhysWorld->CreateRope(m_pGem->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 0.0f, 0.0f }, { 30.0f, 0.0f }, false);
	pRope = new GDI_Obj_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::PURPLE);
	m_pRopes_Cuttable->push_back(pRope);

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
	physProps.pos = { 940, 60 };
	physProps.collisionType = CT_WINZONE;
	physProps.collideWith = (CT_GEM);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pWinZone->Initialise(pTempBody, colorRef::GREEN, colorRef::GREEN));

	/*
		Static level design
	*/

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
	physProps.density = 1.0f;
	physProps.friction = 0.3f;
	physProps.pos = { 880, 290 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -40.0f, -20.0f };
	pPoints[1] = { 40.0f, -20.0f };
	pPoints[2] = { 40.0f, 20.0f };
	pPoints[3] = { -40.0f, 20.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.density = 1.0f;
	physProps.friction = 0.3f;
	physProps.pos = { 820, 660 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -140.0f, -20.0f };
	pPoints[1] = { 140.0f, -20.0f };
	pPoints[2] = { 140.0f, 20.0f };
	pPoints[3] = { -140.0f, 20.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.density = 1.0f;
	physProps.friction = 0.3f;
	physProps.pos = { 720, 660 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -225.0f, -10.0f };
	pPoints[1] = { 225.0f, -10.0f };
	pPoints[2] = { 225.0f, 10.0f };
	pPoints[3] = { -225.0f, 10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.density = 1.0f;
	physProps.friction = 0.3f;
	physProps.pos = { 625, 900 };
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
	physProps.density = 1.0f;
	physProps.friction = 0.3f;
	physProps.pos = { 150, 430 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	/*
		Top See Saw
	*/
	
	GDI_Obj_Generic* pPivot1 = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[3];
	pPoints[0] = { -30.0f, 30.0f };
	pPoints[1] = { 0.0f, -25.0f };
	pPoints[2] = { 30.0f, 30.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 3;
	physProps.density = 1.0f;
	physProps.friction = 0.3f;
	physProps.pos = { 820, 250 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pPivot1->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pPivot1);

	GDI_Obj_Generic* pLever1 = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -150.0f, -5.0f };
	pPoints[1] = { 150.0f, -5.0f };
	pPoints[2] = { 150.0f, 5.0f };
	pPoints[3] = { -150.0f, 5.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.density = 1.0f;
	physProps.friction = 0.3f;
	physProps.angle = DegreesToRadians(20);
	physProps.pos = { 820, 220 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pLever1->Initialise(pTempBody, colorRef::CYAN, colorRef::BLACK));
	m_pObjDynamics->push_back(pLever1);

	m_pPhysWorld->CreateRevoluteJoint(pLever1->GetPhysicsBody(), pPivot1->GetPhysicsBody(), { 820, 220 }, false);

	/*
		Bottom See Saw
	*/

	GDI_Obj_Generic* pPivot2 = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[3];
	pPoints[0] = { -30.0f, 30.0f };
	pPoints[1] = { 0.0f, -25.0f };
	pPoints[2] = { 30.0f, 30.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 3;
	physProps.density = 3.0f;
	physProps.friction = 0.3f;
	physProps.pos = { 820, 650 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pPivot2->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pPivot2);

	GDI_Obj_Generic* pLever2 = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -150.0f, -5.0f };
	pPoints[1] = { 150.0f, -5.0f };
	pPoints[2] = { 150.0f, 5.0f };
	pPoints[3] = { -150.0f, 5.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.density = 3.0f;
	physProps.friction = 0.3f;
	physProps.angle = DegreesToRadians(20);
	physProps.pos = { 820, 620 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pLever2->Initialise(pTempBody, colorRef::CYAN, colorRef::BLACK));
	m_pObjDynamics->push_back(pLever2);

	m_pPhysWorld->CreateRevoluteJoint(pLever2->GetPhysicsBody(), pPivot2->GetPhysicsBody(), { 820, 620 }, false);

	// Rope to connect both Levers
	pPhysRope = m_pPhysWorld->CreateRope(pLever1->GetPhysicsBody(), pLever2->GetPhysicsBody(), { -140.0f, 0.0f }, { -140.0f, 0.0f }, false);
	pRope = new GDI_Obj_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::PURPLE);
	m_pRopes_Cuttable->push_back(pRope);

	/*
		Compressed spring in bottom right
	*/

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -50.0f, -10.0f };
	pPoints[1] = { 50.0f, -10.0f };
	pPoints[2] = { 50.0f, 10.0f };
	pPoints[3] = { -50.0f, 10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.density = 150.0f;
	physProps.pos = { 910.0f, 900.0f };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::DARKBLUE, colorRef::BLACK));
	m_pObjDynamics->push_back(pTempObject);

	m_pPhysWorld->CreateSpring(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 0, 0 }, { 910.0f, 1000.0f }, 2, 0.1f, 100.0f);
	
	pPhysRope = m_pPhysWorld->CreateRope(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 0.0f, 0.0f }, { 910.0f, 1000.0f }, false);
	pRope = new GDI_Obj_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::PURPLE);
	m_pRopes_Cuttable->push_back(pRope);

	/*
		Swinging enemy Object in gem path
	*/

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -20.0f, -20.0f };
	pPoints[1] = { 20.0f, -20.0f };
	pPoints[2] = { 20.0f, 20.0f };
	pPoints[3] = { -20.0f, 20.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.density = 15.0f;
	physProps.pos = { 340.0f, 50.0f };
	physProps.collisionType = CT_ENEMY;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::RED, colorRef::BLACK));
	m_pObjDynamics->push_back(pTempObject);

	pPhysRope = m_pPhysWorld->CreateRope(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 0.0f, -20.0f }, { 500.0f, 50.0f }, false);
	pRope = new GDI_Obj_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::GREY);
	m_pRopes_Unbreakable->push_back(pRope);

	/*
		Bottom left free spring
	*/

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -100.0f, -5.0f };
	pPoints[1] = { 100.0f, -5.0f };
	pPoints[2] = { 100.0f, 5.0f };
	pPoints[3] = { -100.0f, 5.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = false;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 280, 800 };
	physProps.density = 2.0f;
	physProps.friction = 0.3f;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::DARKBLUE, colorRef::BLACK));
	m_pObjDynamics->push_back(pTempObject);

	m_pPhysWorld->CreateSpring(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 0, 0 }, { 280.0f, 1000.0f }, 28, 0.9f);

	/*
		Battering Ram
	*/

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -50.0f, -20.0f };
	pPoints[1] = { 50.0f, -20.0f };
	pPoints[2] = { 50.0f, 20.0f };
	pPoints[3] = { -50.0f, 20.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 90, 500 };
	physProps.density = 1000.0f;
	physProps.friction = 0.3f;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::CYAN, colorRef::BLACK));
	m_pObjDynamics->push_back(pTempObject);

	pPhysRope = m_pPhysWorld->CreateRope(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { -45.0f, -20.0f }, { 175.0f, 430.0f }, false);
	pRope = new GDI_Obj_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::GREY);
	m_pRopes_Unbreakable->push_back(pRope);

	pPhysRope = m_pPhysWorld->CreateRope(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { 45.0f, -20.0f }, { 265.0f, 430.0f }, false);
	pRope = new GDI_Obj_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::GREY);
	m_pRopes_Unbreakable->push_back(pRope);

	pPhysRope = m_pPhysWorld->CreateRope(pTempObject->GetPhysicsBody(), m_pBackground->GetPhysicsBody(), { -45.0f, 0.0f }, { 0.0f, 500.0f }, false);
	pRope = new GDI_Obj_Rope(m_pGDI_Renderer);
	pRope->Initialise(pPhysRope, colorRef::PURPLE);
	m_pRopes_Cuttable->push_back(pRope);

	/*
		Bouncing Boulder
	*/

	GDI_Obj_Generic* m_pCircle = new GDI_Obj_Circle(m_pGDI_Renderer);
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = true;
	physProps.pos = { 280, 500 };
	physProps.radius = 25.0f;
	physProps.density = 450.0f;
	physProps.friction = 0.3f;
	physProps.damping = 0.5f;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pCircle->Initialise(pTempBody, colorRef::CYAN, colorRef::BLACK));
	m_pObjDynamics->push_back(m_pCircle);


	return true;
}
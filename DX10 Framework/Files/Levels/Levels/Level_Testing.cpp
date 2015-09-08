/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Level_Testing.h
* Description : Builds and runs Level 01 for the game
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "Level_Testing.h"

Level_Testing::Level_Testing(GDI_Renderer* _pGDI_Renderer, int _clientWidth, int _clientHeight)
{
	m_pGDI_Renderer = _pGDI_Renderer;
	m_clientWidth = _clientWidth;
	m_clientHeight = _clientHeight;
}

Level_Testing::~Level_Testing()
{
}

bool Level_Testing::ContructLevel()
{
	m_pObjStatics = new std::vector < GDI_Obj_Generic* > ;
	m_pObjDynamics = new std::vector < GDI_Obj_Generic* > ;
	m_pObjBreakables = new std::vector < GDI_Obj_Generic* > ;
	m_pObjRopes = new std::vector < GDI_Rope* > ;
	m_pObjPulleys = new std::vector < GDI_Pulley* > ;
	m_pObjSprings = new std::vector < GDI_Spring* > ;

	// Initialise the Physics system
	m_pPhysWorld = new Physics_World_2D();
	VALIDATE(m_pPhysWorld->Initialise(40.0f));

	// Initialise the Objects
	Physics_Body_2D* pTempBody;
	TPhysicsProperties physProps;
	v2float* pPoints;
	GDI_Obj_Generic* pTempObject;

	m_pGem = new GDI_Obj_Circle(m_pGDI_Renderer);
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = true;
	physProps.pos = { 50, 100 };
	physProps.radius = 15.0f;
	physProps.density = 10.0f;
	physProps.friction = 0.3f;
	physProps.collisionType = CT_GEM;
	physProps.collideWith = (CT_STANDARD | CT_WINZONE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pGem->Initialise(pTempBody, colorRef::PURPLE, colorRef::BLACK));

	m_pWinZone = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -40.0f, -40.0f };
	pPoints[1] = { 40.0f, -40.0f };
	pPoints[2] = { 40.0f, 40.0f };
	pPoints[3] = { -40.0f, 40.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 250, 400 };
	physProps.collisionType = CT_WINZONE;
	physProps.collideWith = (CT_GEM);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pWinZone->Initialise(pTempBody, colorRef::GREEN, colorRef::GREEN));






	m_pBackground = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -1000.0f, -1000.0f };
	pPoints[1] = { 1000.0f, -1000.0f };
	pPoints[2] = { 1000.0f, 1000.0f };
	pPoints[3] = { -1000.0f, 1000.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.circleObject = false;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 0, 0 };
	physProps.collisionType = CT_BACKGROUND;
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pBackground->Initialise(pTempBody, colorRef::WHITE, colorRef::WHITE));

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -100.0f, -25.0f };
	pPoints[1] = { 100.0f, -25.0f };
	pPoints[2] = { 100.0f, 25.0f };
	pPoints[3] = { -100.0f, 25.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = false;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 500, 500 };
	physProps.density = 1.0f;
	physProps.friction = 0.3f;
	physProps.collisionType = CT_BREAKABLE;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLUE, colorRef::BLACK));
	m_pObjBreakables->push_back(pTempObject);

	m_pPhysWorld->CreateRevoluteJoint(m_pBackground->GetPhysicsBody(), pTempObject->GetPhysicsBody(), { 405, 500 }, false);
	m_pPhysWorld->CreateRevoluteJoint(m_pBackground->GetPhysicsBody(), pTempObject->GetPhysicsBody(), { 595, 500 }, false);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[3];
	pPoints[0] = { -20.0f, 20.0f };
	pPoints[1] = { 0.0f, -15.0f };
	pPoints[2] = { 20.0f, 20.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = false;
	physProps.pPoints = pPoints;
	physProps.size = 3;
	physProps.pos = { 500, 600 };
	physProps.density = 300.0f;
	physProps.friction = 0.3f;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLUE, colorRef::BLACK));
	m_pObjDynamics->push_back(pTempObject);

	GDI_Rope* pRope = new GDI_Rope(m_pGDI_Renderer);
	Physics_Rope_2D* pRopeJoint = m_pPhysWorld->CreateRopeJoint(m_pBackground->GetPhysicsBody(), pTempObject->GetPhysicsBody(), { 300, 500 }, { 0, 0 }, false);
	VALIDATE(pRope->Initialise(pRopeJoint, colorRef::RED));
	m_pObjRopes->push_back(pRope);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -25.0f, -100.0f };
	pPoints[1] = { 25.0f, -100.0f };
	pPoints[2] = { 25.0f, 100.0f };
	pPoints[3] = { -25.0f, 100.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = false;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 200, 700 };
	physProps.density = 1.0f;
	physProps.friction = 0.3f;
	//physProps.angle = DegreesToRadians(90.0f);
	physProps.collisionType = CT_BREAKABLE;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLUE, colorRef::BLACK));
	m_pObjBreakables->push_back(pTempObject);

	m_pPhysWorld->CreateRevoluteJoint(m_pBackground->GetPhysicsBody(), pTempObject->GetPhysicsBody(), { 405, 500 }, false);
	m_pPhysWorld->CreateRevoluteJoint(m_pBackground->GetPhysicsBody(), pTempObject->GetPhysicsBody(), { 595, 500 }, false);

	GDI_Obj_Generic* m_pCircle = new GDI_Obj_Circle(m_pGDI_Renderer);
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = true;
	physProps.pos = { 570, 300 };
	physProps.radius = 25.0f;
	physProps.density = 280.0f;
	physProps.friction = 0.3f;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pCircle->Initialise(pTempBody, colorRef::YELLOW, colorRef::BLACK));
	m_pObjDynamics->push_back(m_pCircle);

	// BETTER ROPE JOINT - currently not working
	//std::vector<Physics_Body_2D*>* pRopeBodies = m_pPhysics2D->CreateRope(m_pBackground->GetPhysicsBody(), pTempBody, { 500, 500 }, { 0, 0 }, 0xFF00FF);
	//GDI_Obj_Group* pGroupObj = new GDI_Obj_Group();
	//VALIDATE(pGroupObj->Initialise(m_pGDIRenderer, pRopeBodies, 0xFF00FF));
	//m_groupObjects.push_back(pGroupObj);

	GDI_Obj_Polygon* pPulleyPolyA = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -20.0f, -20.0f };
	pPoints[1] = { 20.0f, -20.0f };
	pPoints[2] = { 20.0f, 20.0f };
	pPoints[3] = { -20.0f, 20.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = false;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 700, 200 };
	physProps.density = 1.0f;
	physProps.friction = 0.3f;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pPulleyPolyA->Initialise(pTempBody, colorRef::BLUE, colorRef::BLACK));
	m_pObjDynamics->push_back(pPulleyPolyA);

	GDI_Obj_Polygon* pPulleyPolyB = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -20.0f, -20.0f };
	pPoints[1] = { 20.0f, -20.0f };
	pPoints[2] = { 20.0f, 20.0f };
	pPoints[3] = { -20.0f, 20.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = false;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 900, 200 };
	physProps.density = 1.1f;
	physProps.friction = 0.3f;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pPulleyPolyB->Initialise(pTempBody, colorRef::BLUE, colorRef::BLACK));
	m_pObjDynamics->push_back(pPulleyPolyB);

	Physics_Pulley_2D* pTempPhysPulley = m_pPhysWorld->CreatePulley(pPulleyPolyA->GetPhysicsBody(), pPulleyPolyB->GetPhysicsBody(), { 0, -20 }, { 0, -20 }, { 700, 100 }, { 900, 100 });
	GDI_Pulley* pTempPulley = new GDI_Pulley(m_pGDI_Renderer, m_pPhysWorld);
	VALIDATE(pTempPulley->Initialise(pTempPhysPulley, colorRef::BLACK));
	m_pObjPulleys->push_back(pTempPulley);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -10.0f, -10.0f };
	pPoints[1] = { 10.0f, -10.0f };
	pPoints[2] = { 10.0f, 10.0f };
	pPoints[3] = { -10.0f, 10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 700, 150 };
	physProps.density = 10.0f;
	physProps.friction = 0.3f;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLUE, colorRef::BLACK));
	m_pObjDynamics->push_back(pTempObject);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -20.0f, -20.0f };
	pPoints[1] = { 20.0f, -20.0f };
	pPoints[2] = { 20.0f, 20.0f };
	pPoints[3] = { -20.0f, 20.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = false;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 750, 500 };
	physProps.density = 10.0f;
	physProps.friction = 0.3f;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLUE, colorRef::BLACK));
	m_pObjDynamics->push_back(pTempObject);

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
	physProps.pos = { 500, 800 };
	physProps.density = 2.0f;
	physProps.friction = 0.3f;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::RED, colorRef::BLACK));
	m_pObjDynamics->push_back(pTempObject);

	GDI_Spring* pSpring = new GDI_Spring(m_pGDI_Renderer);
	Physics_Spring_2D* pPhysSpring = m_pPhysWorld->CreateSpring(m_pBackground->GetPhysicsBody(), pTempBody, { 500, 900 }, { 0, 0 }, 20.0f, 0.9f);
	VALIDATE(pSpring->Initialise(pPhysSpring, colorRef::BLACK));
	m_pObjSprings->push_back(pSpring);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -100.0f, -5.0f };
	pPoints[1] = { 100.0f, -5.0f };
	pPoints[2] = { 100.0f, 5.0f };
	pPoints[3] = { -100.0f, 5.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 800, 850 };
	physProps.density = 2.0f;
	physProps.friction = 0.3f;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	m_pCircle = new GDI_Obj_Circle(m_pGDI_Renderer);
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.circleObject = true;
	physProps.pos = { 101, 100 };
	physProps.radius = 25.0f;
	physProps.density = 100.0f;
	physProps.friction = 0.3f;
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pCircle->Initialise(pTempBody, colorRef::BLUE, colorRef::BLACK));
	m_pObjDynamics->push_back(m_pCircle);

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -200.0f, -5.0f };
	pPoints[1] = { 200.0f, -5.0f };
	pPoints[2] = { 200.0f, 5.0f };
	pPoints[3] = { -200.0f, 5.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 200, 400 };
	physProps.density = 2.0f;
	physProps.friction = 0.3f;
	physProps.angle = DegreesToRadians(45);
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLUE, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	return true;
}
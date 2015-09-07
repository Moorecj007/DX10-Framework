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
	m_pGDI_Renderer = _pGDI_Renderer;
	m_clientWidth = _clientWidth;
	m_clientHeight = _clientHeight;
}

Level_01::~Level_01()
{
	DestroyLevel();
}

bool Level_01::ContructLevel()
{
	TPhysicsProperties physProps;
	v2float* pPoints;
	Physics_Body_2D* pTempBody;
	GDI_Obj_Generic* pTempObject;

	VALIDATE(InitialSetup());

	/*
		Create Win Zone
	*/

	m_pWinZone = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -50.0f, -20.0f };
	pPoints[1] = {  50.0f, -20.0f };
	pPoints[2] = {  50.0f,  20.0f };
	pPoints[3] = { -50.0f,  20.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 920, 930 };
	physProps.collisionType = CT_WINZONE;
	physProps.collideWith = (CT_GEM);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pWinZone->Initialise(pTempBody, colorRef::GREEN, colorRef::GREEN));

	/* 
		Create the Gem
	*/

	/*
		Create Standard Static Level Layout
	*/

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -50.0f, -10.0f };
	pPoints[1] = {  50.0f, -10.0f };
	pPoints[2] = {  50.0f,  10.0f };
	pPoints[3] = { -50.0f,  10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.pos = { 920, 890 };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pTempObject);

	//pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	//pPoints = new v2float[4];
	//pPoints[0] = { -100.0f, -10.0f };
	//pPoints[1] = {  100.0f, -10.0f };
	//pPoints[2] = {  100.0f,  10.0f };
	//pPoints[3] = { -100.0f,  10.0f };
	//ZeroMemory(&physProps, sizeof(physProps));
	//physProps.isStatic = true;
	//physProps.pPoints = pPoints;
	//physProps.size = 4;
	//physProps.pos = { 920, 890 };
	//physProps.collisionType = CT_STANDARD;
	//physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	//pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	//VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	//m_pObjStatics->push_back(pTempObject);

	/*
		Create the Pulley System
	*/

	GDI_Obj_Generic* pPulleySystemA = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -20.0f, -50.0f };
	pPoints[1] = {  20.0f, -50.0f };
	pPoints[2] = {  20.0f,  50.0f };
	pPoints[3] = { -20.0f,  50.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.density = 1.0f;
	physProps.size = 4;
	physProps.pos = { 840, 898 };
	physProps.collisionType = CT_ENEMY;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pPulleySystemA->Initialise(pTempBody, colorRef::RED, colorRef::BLACK));
	m_pObjDynamics->push_back(pPulleySystemA);

	GDI_Obj_Generic* pPulleySystemB = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -50.0f, -50.0f };
	pPoints[1] = {  50.0f, -50.0f };
	pPoints[2] = {  50.0f,  50.0f };
	pPoints[3] = { -50.0f,  50.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.density = 1.0f;
	physProps.size = 4;
	physProps.pos = { 100, 600 };
	physProps.collisionType = CT_ENEMY;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pPulleySystemB->Initialise(pTempBody, colorRef::RED, colorRef::BLACK));
	m_pObjDynamics->push_back(pPulleySystemB);

	Physics_Pulley_2D* pTempPhysPulley = m_pPhysWorld->CreatePulley(pPulleySystemA->GetPhysicsBody(), pPulleySystemB->GetPhysicsBody(), { 0, -50.0f }, { 0, -50.0f }, { 840, 100 }, { 100, 100 });
	GDI_Pulley* pTempPulley = new GDI_Pulley(m_pGDI_Renderer, m_pPhysWorld);
	VALIDATE(pTempPulley->Initialise(pTempPhysPulley, colorRef::BLACK));
	m_pObjPulleys->push_back(pTempPulley);

	/*
		Create the Breakable objects
	*/

	pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -10.0f, -100.0f };
	pPoints[1] = {  10.0f, -100.0f };
	pPoints[2] = {  10.0f,  100.0f };
	pPoints[3] = { -10.0f,  100.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.pPoints = pPoints;
	physProps.density = 1.0f;
	physProps.friction = 0.3f;
	physProps.size = 4;
	physProps.pos = { 100, 800 };
	physProps.collisionType = CT_BREAKABLE;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pTempObject->Initialise(pTempBody, colorRef::BLUE, colorRef::BLACK));
	m_pObjBreakables->push_back(pTempObject);

	return true;
}
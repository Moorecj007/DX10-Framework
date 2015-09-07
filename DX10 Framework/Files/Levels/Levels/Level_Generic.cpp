/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Level_Generic.cpp
* Description : Base class for all levels to be derived from
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "Level_Generic.h"

Level_Generic::~Level_Generic()
{
}

void Level_Generic::Process(float _dt)
{
	m_pPhysWorld->Process();

	m_pBackground->Process(_dt);
	m_pWinZone->Process(_dt);
	m_pGem->Process(_dt);

	TCollisionProperties collisionProps = *m_pGem->GetPhysicsBody()->GetCollisionProperties();
	if (collisionProps.isLevelLost == true)
	{
		ResetLevel();
		return;
	}
	if (collisionProps.isLevelWon == true)
	{
		ResetLevel();
		return;
	}

	// Process all lines
	for (UINT i = 0; i < m_pObjRopes->size(); i++)
	{
		(*m_pObjRopes)[i]->Process(_dt);
	}
	// Process all Pulleys
	for (UINT i = 0; i < m_pObjPulleys->size(); i++)
	{
		(*m_pObjPulleys)[i]->Process(_dt);
	}
	// Process all static objects
	for (UINT i = 0; i < m_pObjStatics->size(); i++)
	{
		(*m_pObjStatics)[i]->Process(_dt);
	}
	// Process all dynamic objects
	for (UINT i = 0; i < m_pObjDynamics->size(); i++)
	{
		(*m_pObjDynamics)[i]->Process(_dt);
	}
	// Process all Springs
	for (UINT i = 0; i < m_pObjSprings->size(); i++)
	{
		(*m_pObjSprings)[i]->Process(_dt);
	}

	std::vector<UINT> deletedObjectIndices;
	std::vector<GDI_Obj_Generic*> createdObjects;

	// Process all breakable objects
	for (UINT i = 0; i < m_pObjBreakables->size(); i++)
	{
		(*m_pObjBreakables)[i]->Process(_dt);
		TCollisionProperties* collisionProps = (*m_pObjBreakables)[i]->GetPhysicsBody()->GetCollisionProperties();

		// Determine if the object needs to be broken
		if (collisionProps->isBreaking == true)
		{
			// Break the object
			std::vector<Physics_Body_2D*>* pNewBodies = m_pPhysWorld->BreakObject((*m_pObjBreakables)[i]->GetPhysicsBody());

			COLORREF colorFill = (*m_pObjBreakables)[i]->GetColorFill();
			COLORREF colorOutline = (*m_pObjBreakables)[i]->GetColorOutline();

			// Save the index of the object that broke so it can be deleted safely later
			deletedObjectIndices.push_back(i);

			// Create new GDI objects for all the new created bodies of the broken object
			for (UINT newIndex = 0; newIndex < pNewBodies->size(); newIndex++)
			{
				GDI_Obj_Generic* pTempObject = new GDI_Obj_Polygon(m_pGDI_Renderer);
				pTempObject->Initialise((*pNewBodies)[newIndex], colorFill, colorOutline);
				createdObjects.push_back(pTempObject);
			}

			ReleasePtr(pNewBodies);
		}
	}

	// If a object was broken
	if (deletedObjectIndices.size() > 0)
	{
		// Delete all the objects in backwards order to safely use indices
		for (int i = (int)deletedObjectIndices.size() - 1; i >= 0; i--)
		{
			ReleasePtr((*m_pObjBreakables)[i]);
			m_pObjBreakables->erase(m_pObjBreakables->begin() + i);
		}

		// add all created objects to the dynamics objects vector
		for (UINT i = 0; i < createdObjects.size(); i++)
		{
			m_pObjDynamics->push_back(createdObjects[i]);
		}
	}
}

bool Level_Generic::InitialSetup()
{
	m_pObjStatics = new std::vector < GDI_Obj_Generic* >;
	m_pObjDynamics = new std::vector < GDI_Obj_Generic* >;
	m_pObjBreakables = new std::vector < GDI_Obj_Generic* >;
	m_pObjRopes = new std::vector < GDI_Rope* >;
	m_pObjPulleys = new std::vector < GDI_Pulley* >;
	m_pObjSprings = new std::vector < GDI_Spring* >;

	m_pPhysWorld = new Physics_World_2D();
	VALIDATE(m_pPhysWorld->Initialise(40.0f));

	TPhysicsProperties physProps;
	v2float* pPoints;
	Physics_Body_2D* pTempBody;

	// Create a background object that is twice the size of the client window and height
	// This allows the center to be on the origin so world space coordinate are the same on this object
	m_pBackground = new GDI_Obj_Polygon(m_pGDI_Renderer);
	ZeroMemory(&physProps, sizeof(physProps));
	pPoints = new v2float[4];
	pPoints[0] = { (float)(-m_clientWidth * 2), (float)(-m_clientHeight * 2) };
	pPoints[1] = { (float)(m_clientWidth * 2), (float)(-m_clientHeight * 2) };
	pPoints[2] = { (float)(m_clientWidth * 2), (float)(m_clientHeight * 2) };
	pPoints[3] = { (float)(-m_clientWidth * 2), (float)(m_clientHeight * 2) };
	physProps.pPoints = pPoints;
	physProps.isStatic = true;
	physProps.pos = { 0.0f, 0.0f };
	physProps.size = 4;
	physProps.collisionType = CT_BACKGROUND;
	physProps.collideWith = 0;
	Physics_Body_2D* pBackgroundBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(m_pBackground->Initialise(pBackgroundBody, colorRef::PURPLE, colorRef::WHITE));

	/*
	Create the four walls of the level
	*/

	GDI_Obj_Generic* pNorthWall = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { (float)(-m_clientWidth / 2), -10.0f };
	pPoints[1] = { (float)(m_clientWidth / 2), -10.0f };
	pPoints[2] = { (float)(m_clientWidth / 2), 10.0f };
	pPoints[3] = { (float)(-m_clientWidth / 2), 10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.friction = 0.3f;
	physProps.pos = { (float)(m_clientWidth / 2.0f), 0.0f };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pNorthWall->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pNorthWall);

	GDI_Obj_Generic* pSouthWall = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { (float)(-m_clientWidth / 2), -10.0f };
	pPoints[1] = { (float)(m_clientWidth / 2), -10.0f };
	pPoints[2] = { (float)(m_clientWidth / 2), 10.0f };
	pPoints[3] = { (float)(-m_clientWidth / 2), 10.0f };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.friction = 0.3f;
	physProps.pos = { (float)(m_clientWidth / 2.0f), (float)(m_clientHeight)-40.0f };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pSouthWall->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pSouthWall);

	GDI_Obj_Generic* pEastWall = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -10.0f, (float)(-m_clientHeight / 2) };
	pPoints[1] = { 10.0f, (float)(-m_clientHeight / 2) };
	pPoints[2] = { 10.0f, (float)(m_clientHeight / 2) };
	pPoints[3] = { -10.0f, (float)(m_clientHeight / 2) };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.friction = 0.3f;
	physProps.pos = { 0.0f, (float)(m_clientHeight / 2.0f) };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pEastWall->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pEastWall);

	GDI_Obj_Generic* pWestWall = new GDI_Obj_Polygon(m_pGDI_Renderer);
	pPoints = new v2float[4];
	pPoints[0] = { -10.0f, (float)(-m_clientHeight / 2) };
	pPoints[1] = { 10.0f, (float)(-m_clientHeight / 2) };
	pPoints[2] = { 10.0f, (float)(m_clientHeight / 2) };
	pPoints[3] = { -10.0f, (float)(m_clientHeight / 2) };
	ZeroMemory(&physProps, sizeof(physProps));
	physProps.isStatic = true;
	physProps.pPoints = pPoints;
	physProps.size = 4;
	physProps.friction = 0.3f;
	physProps.pos = { (float)(m_clientWidth)-16.0f, (float)(m_clientHeight / 2.0f) };
	physProps.collisionType = CT_STANDARD;
	physProps.collideWith = (CT_STANDARD | CT_GEM | CT_BREAKABLE | CT_ENEMY);
	pTempBody = m_pPhysWorld->CreatePhysicsObject(physProps);
	VALIDATE(pWestWall->Initialise(pTempBody, colorRef::BLACK, colorRef::BLACK));
	m_pObjStatics->push_back(pWestWall);

	return true;
}

void Level_Generic::DestroyLevel()
{
	// Delete all GDI Ropes
	for (UINT i = 0; i < m_pObjRopes->size(); i++)
	{
		ReleasePtr((*m_pObjRopes)[i]);
	}
	ReleasePtr(m_pObjRopes);
	// Delete all GDI Pulley Objects
	for (UINT i = 0; i < m_pObjPulleys->size(); i++)
	{
		ReleasePtr((*m_pObjPulleys)[i]);
	}
	ReleasePtr(m_pObjPulleys);
	// Delete all GDI Spring Objects
	for (UINT i = 0; i < m_pObjSprings->size(); i++)
	{
		ReleasePtr((*m_pObjSprings)[i]);
	}
	ReleasePtr(m_pObjSprings);
	// Delete all GDI static objects
	for (UINT i = 0; i < m_pObjStatics->size(); i++)
	{
		ReleasePtr((*m_pObjStatics)[i]);
	}
	ReleasePtr(m_pObjStatics);
	// Delete all GDI dynamic objects
	for (UINT i = 0; i < m_pObjDynamics->size(); i++)
	{
		ReleasePtr((*m_pObjDynamics)[i]);
	}
	ReleasePtr(m_pObjDynamics);
	// Delete all GDI breakable objects
	for (UINT i = 0; i < m_pObjBreakables->size(); i++)
	{
		ReleasePtr((*m_pObjBreakables)[i]);
	}
	ReleasePtr(m_pObjBreakables);

	ReleasePtr(m_pGem);
	ReleasePtr(m_pWinZone);
	ReleasePtr(m_pBackground);
	ReleasePtr(m_pPhysWorld);
}

void Level_Generic::ResetLevel()
{
	DestroyLevel();
	ContructLevel();
}

void Level_Generic::Render()
{
	m_pBackground->Render();
	m_pWinZone->Render();

	// Render all lines
	for (UINT i = 0; i < m_pObjRopes->size(); i++)
	{
		(*m_pObjRopes)[i]->Render();
	}
	// Render all Pulley objects
	for (UINT i = 0; i < m_pObjPulleys->size(); i++)
	{
		(*m_pObjPulleys)[i]->Render();
	}
	// Render all static objects
	for (UINT i = 0; i < m_pObjStatics->size(); i++)
	{
		(*m_pObjStatics)[i]->Render();
	}
	// Render all dynamic objects
	for (UINT i = 0; i < m_pObjDynamics->size(); i++)
	{
		(*m_pObjDynamics)[i]->Render();
	}
	// Render all Spring objects
	for (UINT i = 0; i < m_pObjSprings->size(); i++)
	{
		(*m_pObjSprings)[i]->Render();
	}
	// Render all breakable objects
	for (UINT i = 0; i < m_pObjBreakables->size(); i++)
	{
		(*m_pObjBreakables)[i]->Render();
	}

	m_pGem->Render();
}

void Level_Generic::CutRope(v2float _cutLinePtA, v2float _cutLinePtB)
{
	// Cycle through all ropes backwards so ropes can be deleted without changing the indices
	for (int i = (int)m_pObjRopes->size() - 1; i >= 0; i--)
	{
		v2float ropePtA = (*m_pObjRopes)[i]->GetPositionA();
		v2float ropePtB = (*m_pObjRopes)[i]->GetPositionB();

		if (CheckLinesIntersect(_cutLinePtA, _cutLinePtB, ropePtA, ropePtB))
		{
			ReleasePtr((*m_pObjRopes)[i]);
			m_pObjRopes->erase(m_pObjRopes->begin() + i);
		}
	}
}



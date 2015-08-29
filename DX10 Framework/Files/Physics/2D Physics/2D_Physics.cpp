/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : 2D_Physics.cpp
* Description : Simulates all 2D physics for the application
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "2D_Physics.h"

C2D_Physics::C2D_Physics()
{
}

C2D_Physics::~C2D_Physics()
{
	//ReleasePtr(m_pWorld);
}

bool C2D_Physics::Initialise()
{
	//b2Vec2 gravity(0.0f, -9.81f);
	//bool allowSleep = true;
	//
	//// Create a new World
	//m_pWorld = new b2World(gravity);
	//m_pWorld->SetAllowSleeping(allowSleep);

	return true;
}

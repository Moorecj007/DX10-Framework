/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : 2D_Physics.h
* Description : Simulates all 2D physics for the application
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __2D_PHYSICS_H__
#define __2D_PHYSICS_H__

// Library Includes
//#pragma comment(lib, "Box2D.lib")
//#include <Box2D.h>

// Local Includes
#include "../../Utility/Utilities.h"

class C2D_Physics
{
public:

	/***********************
	* C2D_Physics: Default Constructor for 2D Physics class
	* @author: Callan Moore
	********************/
	C2D_Physics();

	/***********************
	* ~C2D_Physics: Default Destructor for 2D Physics class
	* @author: Callan Moore
	********************/
	~C2D_Physics();

	// TO DO
	bool Initialise();

private:
	//b2World* m_pWorld;
};

#endif	// __2D_PHYSICS_H__




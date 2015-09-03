/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Rope_2D.h
* Description : Simulated all physics for a rope object
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __PHYSICS_ROPE_2D_H__
#define __PHYSICS_ROPE_2D_H__

// Local Includes
#include "Physics_Utilities.h"

class Physics_Rope_2D
{
public:

	/***********************
	* Physics_Rope_2D: Default Constructor for 2D Physics Rope class
	* @author: Callan Moore
	********************/
	Physics_Rope_2D();

	/***********************
	* ~Physics_Rope_2D: Default Destructor for 2D Physics Rope class
	* @author: Callan Moore
	********************/
	~Physics_Rope_2D();

	// TO DO
	bool Initialise();
};
#endif	// __PHYSICS_ROPE_2D_H__


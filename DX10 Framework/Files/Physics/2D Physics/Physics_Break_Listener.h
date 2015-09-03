/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Break_Listener.h
* Description : Contact listener that detects objects that need to break
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __PHYSICS_BREAK_LISTENER_H__
#define __PHYSICS_BREAK_LISTENER_H__

// Library Includes
#include <Box2D.h>

// Local Includes
#include "Physics_Utilities.h"
#include "Physics_Body_2D.h"

class Physics_Break_Listener :
	public b2ContactListener
{
public:
	/***********************
	* Physics_Break_Listener: Default Constructor for Break Listener class
	* @author: Callan Moore
	********************/
	Physics_Break_Listener();

	/***********************
	* ~Physics_Break_Listener: Default Destructor for Break Listener class
	* @author: Callan Moore
	********************/
	~Physics_Break_Listener();

	// TO DO
	void BeginContact(b2Contact* _contact);
	void EndContact(b2Contact* _contact);
	void PreSolve(b2Contact* _contact, const b2Manifold* _oldManifold);
	void PostSolve(b2Contact* _contact, const b2ContactImpulse* _impulse);
};

#endif	// __PHYSICS_BREAK_LISTENER_H__


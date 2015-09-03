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
#ifndef __PHYSICS_UTILITIES_H__
#define __PHYSICS_UTILITIES_H__

// Library Includes
#pragma comment(lib, "Box2D.lib")
#include <Box2D.h>

// Local Includes
#include "../../Utility/Utilities.h"

/***********************
* TPhysicsProperties: Properties struct for creating a physics body
* @author: Callan Moore
********************/
struct TPhysicsProperties
{
	bool circleObject;
	float radius;

	v2float* pPoints;
	UINT size;

	v2float	pos;
	float density;
	float friction;
	float restitution;
	float angle;
	UINT16 collisionType;
	UINT16 collideWith;
};

/***********************
* TBreakProperties: Properties struct for an object that needs to break
* @author: Callan Moore
********************/
struct TBreakProperties
{
	bool broken;
	v2float* pCollisionWorldPoints;
	v2float impactVelocity;
};

/***********************
* eCollisionType: enum for collision types
* @author: Callan Moore
********************/
enum eCollisionType
{
	CT_BACKGROUND = 0x0001,
	CT_STATIC = 0x0002,
	CT_DYNAMIC = 0x0004,
	CT_BREAKABLE = 0x0008,
};

#endif	// __PHYSICS_UTILITIES_H__
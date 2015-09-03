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

// Local Includes
#include "../../Utility/Utilities.h"

/***********************
* TPhysicsProperties: Properties struct for creating a physics body
* @author: Callan Moore
********************/
struct TPhysicsProperties
{
	v2float* pPoints;
	UINT size;
	v2float	pos;
	v2float scale;
	float density;
	float friction;
	float restitution;
	float angle;
	UINT16 collisionType;
	UINT16 collideWith;
};

struct TBreakProperties
{
	bool broken;
	v2float* pCollisionWorldPoints;
	v2float impactVelocity;
};

// enum
enum eCollisionType
{
	CT_BACKGROUND = 0x0001,
	CT_STATIC = 0x0002,
	CT_DYNAMIC = 0x0004,
	CT_BREAKABLE = 0x0008,
};

#endif	// __PHYSICS_UTILITIES_H__
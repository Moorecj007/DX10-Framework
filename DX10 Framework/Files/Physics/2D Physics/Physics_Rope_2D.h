/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Rope_2D.h
* Description : A physics implementation of a rope
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __PHYSICS_JOINT_2D_H__
#define __PHYSICS_JOINT_2D_H__

// Library Includes
#include <Box2D.h>

// Local Includes
#include "../../Utility/Utilities.h"

class Physics_Rope_2D
{
public:
	
	/***********************
	* Physics_Rope_2D: Constructor for 2D Physics Rope class
	* @author: Callan Moore
	* @parameter: _pJoint: The Box2D physics joint to govern this objects physics interactions
	* @parameter: _pixelsPerMeter: The conversion rate of meters to screen space
	********************/
	Physics_Rope_2D(b2Joint* _pJoint, float _pixelsPerMeter);

	/***********************
	* ~Physics_Rope_2D: Default Destructor for 2D Physics Rope class
	* @author: Callan Moore
	********************/
	~Physics_Rope_2D();

	/***********************
	* GetAnchorA: Retrieve the first anchor point
	* @author: Callan Moore
	* @return: v2float: The first anchor point in world space
	********************/
	v2float GetAnchorA();

	/***********************
	* GetAnchorB: Retrieve the second anchor point
	* @author: Callan Moore
	* @return: v2float: The second anchor point in world space
	********************/
	v2float GetAnchorB();

private:
	b2Joint* m_pJoint;
	float m_pixelsPerMeter;
};
#endif// __PHYSICS_JOINT_2D_H__


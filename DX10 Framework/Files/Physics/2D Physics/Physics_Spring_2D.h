/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Spring_2D.h
* Description : Physics Representation of a spring
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __PHYSICS_SPRING_2D_H__
#define __PHYSICS_SPRING_2D_H__

// Library Includes
#include <Box2D.h>

class Physics_Spring_2D
{
public:

	/***********************
	* Physics_Spring_2D: Default Constructor for Physics Spring class
	* @author: Callan Moore
	* @parameter: 
	********************/
	Physics_Spring_2D(b2MotorJoint* _pMotorJoint, float _pixelsPerMeter);

	/***********************
	* ~Physics_Spring_2D: Default Destructor for Physics Spring class
	* @author: Callan Moore
	********************/
	~Physics_Spring_2D();

private:
	b2MotorJoint* m_pMotorJoint;
	float m_pixelsPerMeter;
};
#endif	// __PHYSICS_SPRING_2D_H__


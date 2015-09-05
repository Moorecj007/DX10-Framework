/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Spring_2D.cpp
* Description : Physics Representation of a spring
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "Physics_Spring_2D.h"

Physics_Spring_2D::Physics_Spring_2D(b2MotorJoint* _pMotorJoint, float _pixelsPerMeter)
{
	m_pMotorJoint = _pMotorJoint;
	m_pixelsPerMeter = _pixelsPerMeter;
}

Physics_Spring_2D::~Physics_Spring_2D()
{
}

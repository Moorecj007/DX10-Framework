/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Constraint.h
* Description : TO DO CAL
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// Inclusion Guards
#pragma once
#ifndef __PHYSICS_CONTRAINT_H__
#define __PHYSICS_CONTRAINT_H__

// Local Includes
#include "Physics_Particle.h"

class Physics_Constraint
{
public:
	/***********************
	* Physics_Constraint: Default Constructor for Physics Constraint class
	* @author: Callan Moore
	********************/
	Physics_Constraint();

	/***********************
	* ~Physics_Constraint: Default Destructor for Physics Constraint class
	* @author: Callan Moore
	********************/
	~Physics_Constraint();

	// TO DO CAL
	bool Initialise(Physics_Particle* _pA, Physics_Particle* _pB, float _restDist = -1.0f);
	void SatisfyConstraint();

public:
	Physics_Particle* m_pParticleA;
	Physics_Particle* m_pParticleB;
private:
	float m_restDist;


};
#endif	// __PHYSICS_CONTRAINT_H__


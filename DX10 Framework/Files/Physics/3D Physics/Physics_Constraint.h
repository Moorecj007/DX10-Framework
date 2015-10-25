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
	
	/***********************
	* Initialise: Initialise the constraint for use
	* @author: Callan Moore
	* @parameter: _pA: The first particle to constrain
	* @parameter: _pB: The second particle to constrain
	* @parameter: _restDist: The resting distance of the particles (Default: calculates the initial distance as resting distance)
	* @return: bool: Successful or not
	********************/
	bool Initialise(Physics_Particle* _pA, Physics_Particle* _pB, float _restDist = -1.0f);
	
	/***********************
	* SatisfyConstraint: Manipulate the particles and correct them to satisfy the constraints
	* @author: Callan Moore
	* @return: bool: Constraint was broken
	********************/
	bool SatisfyConstraint();

private:
	Physics_Particle* m_pParticleA;
	Physics_Particle* m_pParticleB;
	float m_restDist;

	int m_indexNumA;
	int m_indexNumB;

	bool m_active;
};
#endif	// __PHYSICS_CONTRAINT_H__


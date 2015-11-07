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
	* @parameter: _immediate: Whether the constraint is an immediate constraint
	* @parameter: _breakModifier: Ratio to how far a constraint can stretch before breaking;
	* @return: bool: Successful or not
	********************/
	bool Initialise(Physics_Particle* _pA, Physics_Particle* _pB, bool _immediate, float _breakModifier);
	
	/***********************
	* SatisfyConstraint: Manipulate the particles and correct them to satisfy the constraints
	* @author: Callan Moore
	* @return: bool: Constraint was satisfied, false means constraint was broken
	********************/
	bool SatisfyConstraint();

	// FOR JC
	// TO DO CAL
	bool BurnDown(float _dt, Physics_Particle*& _prParticleToIgnite);
	void Ignite(float _burnTimer) { m_ignited = true; m_burnTimer = _burnTimer; };
	bool GetIgnitedStatus() { return m_ignited; };

private:
	Physics_Particle* m_pParticleA;
	Physics_Particle* m_pParticleB;
	float m_restDist;
	float m_breakDist;
	float m_elasticity;

	bool m_active;
	bool m_immediate;

	// FOR JC
	bool m_ignited;
	float m_burnTimer;
};
#endif	// __PHYSICS_CONTRAINT_H__


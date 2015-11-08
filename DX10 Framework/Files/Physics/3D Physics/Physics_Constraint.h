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

// FOR JC
// Enumerators
enum eConstraintBurning
{
	CB_NOACTION,
	CB_IGNITEOTHERS,
	CB_DESTROYED
};

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

	
	/***********************
	* BurnDown: Process the burning of the constraint
	* @author: Callan Moore
	* @parameter: _dt: The current delta tick (time step) that passed since the last process
	* @parameter: _prParticleToIgnite: Storage variable for a particle if this constraint has fully lit
	* @return: eConstraintBurning: The result of the burn down
	********************/
	eConstraintBurning BurnDown(float _dt, Physics_Particle*& _prParticleToIgnite);
	
	/***********************
	* Ignite: Ignite the constraint and set the burn timer
	* @author: Callan Moore
	* @parameter: _burnTimer: The time for the particle to burn for
	* @return: void
	********************/
	void Ignite(float _burnTimer);
	
	/***********************
	* CanBeIgnited: Check if the constraint can be ignited
	* @author: Callan Moore
	* @return: bool: true if the constraint can be ignited
	********************/
	bool CanBeIgnited() { return (m_active == true && m_ignited == false); };

private:
	Physics_Particle* m_pParticleA;
	Physics_Particle* m_pParticleB;
	float m_restDist;
	float m_breakDist;
	float m_elasticity;

	bool m_active;
	bool m_immediate;

	// FOR JC
	// Ignition Variables
	bool m_ignited;
	float m_burnTime;
	float m_timeUntilIgniteOthers;
	float m_timeUntilDestroyed;
};
#endif	// __PHYSICS_CONTRAINT_H__


/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Constraint.cpp
* Description : Class to calculate physics on a constraint between particles
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "Physics_Constraint.h"

Physics_Constraint::Physics_Constraint()
{
	// Initialise pointers to NULL
	m_pParticleA = 0;
	m_pParticleB = 0;
}

Physics_Constraint::~Physics_Constraint()
{
}

bool Physics_Constraint::Initialise(Physics_Particle* _pA, Physics_Particle* _pB, bool _immediate, float _breakModifier)
{
	if (_pA == 0 || _pB == 0)
	{
		// Pointer is NULL therefore Initialization has failed
		return false;
	}

	// Assign member variables
	m_pParticleA = _pA;
	m_pParticleB = _pB;
	m_active = true;
	m_immediate = _immediate;
	m_elasticity = 0.07f;

	m_ignited = false;
	m_timeUntilIgniteOthers = 0.0f;
	m_timeUntilDestroyed = 0.0f;

	// Calculate based on particle positions
	m_restDist = (*m_pParticleA->GetPosition() - *m_pParticleB->GetPosition()).Magnitude();

	if (m_immediate == false)
	{
		// Edit the modifier value for a secondary constraint
		_breakModifier--;
		_breakModifier /= 2;
		_breakModifier++;
	}
	
	// Calculate the break distance using the modifier
	m_breakDist = m_restDist * _breakModifier;

	return true;
}

bool Physics_Constraint::SatisfyConstraint()
{
	// Check if the calculations need to be run
	if (m_active == true)
	{
		// Calculate the vector from particle A to B and its length
		v3float differenceVec = (*m_pParticleB->GetPosition() - *m_pParticleA->GetPosition());
		float currDist = differenceVec.Magnitude();

		if (currDist > m_breakDist)
		{
			// Distance has exceeded breaking threshold, set the constraint to inactive (broken)
			m_active = false;
			return false;
		}
		else
		{
			// Calculate the maximum distance and the minimum distance the particles can be away from each other
			float minDist = (m_restDist - (m_restDist * m_elasticity));
			float maxDist = (m_restDist + (m_restDist * m_elasticity));

			if (m_immediate == true || currDist > maxDist)
			{
				v3float correctionVec;
				if (currDist > maxDist)
				{
					// Calculate the correction vector needed to return to rest
					correctionVec = differenceVec * (1 - maxDist / currDist);
				}
				else if (currDist < minDist)
				{
					// Calculate the correction vector needed to return to rest
					correctionVec = differenceVec * (1 - minDist / currDist);
				}

				// Halve the correction vector so each particle and can get half
				v3float correctionVecHalved = correctionVec / 2.0f;

				// Move the particles based on the correction vector
				m_pParticleA->Move(correctionVecHalved);
				m_pParticleB->Move(-correctionVecHalved);
			}
		}
	}
	return true;
}

void Physics_Constraint::Ignite(float _burnTimer)
{
	// Set the ignited state of the constraint to true
	m_ignited = true;
	
	if (m_immediate == true)
	{
		// Set the burn time as it was passed in
		m_timeUntilIgniteOthers = _burnTimer;
	}
	else
	{
		// Constraint is secondary (Twice as long) therefore double the burn timer
		m_timeUntilIgniteOthers = _burnTimer * 2.0f;	
	}
	m_timeUntilDestroyed = m_timeUntilIgniteOthers + (_burnTimer * 3);	
}

eIgnitedAction Physics_Constraint::BurnDown(float _dt, Physics_Particle*& _prParticleToIgnite)
{
	// Calculate only if the constraint is ignited and still active
	if (m_ignited == true && m_active == true)
	{
		// Reduce the burning timers
		m_timeUntilIgniteOthers -= _dt;
		m_timeUntilDestroyed -= _dt;

		if ( m_timeUntilIgniteOthers <= 0.0f)
		{
			// Ignite others timer has completed

			if (m_pParticleA->GetIgnitedState() == false)
			{
				// Particle A is the new particle to ignite
				_prParticleToIgnite = m_pParticleA;
			}
			else if (m_pParticleB->GetIgnitedState() == false)
			{
				// Particle B is the new particle to ignite
				_prParticleToIgnite = m_pParticleB;
			}
			
			// Set the time to a large number so that this code is only executed once
			m_timeUntilIgniteOthers = 1000.0f;

			// Tell the Cloth that a new particle is to be ignited
			return IA_IGNITEPARTICLE;
		}
		else if (m_timeUntilDestroyed <= 0.0f)
		{
			// The destroy timer has completed so destroy the constraint
			m_active = false;

			// Tell the Cloth that this constraint has been destroyed
			return IA_DESTROYCONSTRAINT;
		}
	}

	// No additional action needs to happen at this time
	return IA_NOACTION;
}
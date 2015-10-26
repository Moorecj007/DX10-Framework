/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Constraint.cpp
* Description : TO DO CAL
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "Physics_Constraint.h"

Physics_Constraint::Physics_Constraint()
{
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

	// Calculate based on particle positions
	m_restDist = (*m_pParticleA->GetPosition() - *m_pParticleB->GetPosition()).Magnitude();
	
	if (m_immediate == false)
	{
		_breakModifier--;
		_breakModifier /= 2;
		_breakModifier++;
	}
	m_breakDist = m_restDist * _breakModifier;
	m_elasticity = 0.07f;

	return true;
}

bool Physics_Constraint::SatisfyConstraint()
{
	if (m_active == true)
	{
		// Calculate the vector from particle A to B and its length
		v3float differenceVec = (*m_pParticleB->GetPosition() - *m_pParticleA->GetPosition());
		float currDist = differenceVec.Magnitude();

		if (currDist > m_breakDist)
		{
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
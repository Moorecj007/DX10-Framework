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

bool Physics_Constraint::Initialise(Physics_Particle* _pA, Physics_Particle* _pB, bool _immediate, float _restDist)
{
	if (_pA == 0 || _pB == 0)
	{
		// Pointer is NULL therefore Initialization has failed
		return false;
	}

	// Assign member variables
	m_pParticleA = _pA;
	m_pParticleB = _pB;

	// Calculate Rest Distance
	if (_restDist == -1.0f)
	{
		// Distance is not specified, calculate based on particle positions
		m_restDist = (*m_pParticleA->GetPosition() - *m_pParticleB->GetPosition()).Magnitude();
	}
	else
	{
		// Distance is specified
		m_restDist = _restDist;
	}

	m_active = true;
	m_immediate = _immediate;

	return true;
}

bool Physics_Constraint::SatisfyConstraint()
{
	if (m_active == true)
	{
		// Calculate the vector from particle A to B and its length
		v3float differenceVec = (*m_pParticleB->GetPosition() - *m_pParticleA->GetPosition());
		float currDist = differenceVec.Magnitude();

		if (currDist > m_restDist * 3.0f)
		{
			m_active = false;
			return true;
		}
		else
		{	
			bool correct = false;
			if (m_immediate == true)
			{
				correct = true;
			}
			else if (currDist > m_restDist)
			{
				correct = true;
			}

			if (correct == true)
			{
				// Calculate the correction vector needed to return to rest
				v3float correctionVec = differenceVec * (1 - m_restDist / currDist);

				// Halve the correction vector so each particle and can get half
				v3float correctionVecHalved = correctionVec / 2.0f;

				// Move the particles based on the correction vector
				m_pParticleA->Move(correctionVecHalved);
				m_pParticleB->Move(-correctionVecHalved);
			}
		}
	}
	return false;
}
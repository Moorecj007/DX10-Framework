/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Particle.cpp
* Description : TO DO CAL
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "Physics_Particle.h"

Physics_Particle::Physics_Particle()
{
	// FOR JC
	// Initialise all Pointers to NULL
	m_pVertex = 0;
}

Physics_Particle::~Physics_Particle()
{
}

bool Physics_Particle::Initialise(int _particleID, TVertexColor* _pVertex, v3float _pos, float _timeStep, float _damping, bool _static)
{
	// FOR JC
	if (_pVertex == 0)
	{
		// If the vertex is null then the initialisation is invalid
		return false;
	}

	// Assign member variables
	m_particleID = _particleID;
	m_static = _static;
	m_pVertex = _pVertex;
	m_selected = false; // FOR JC

	m_pos = _pos;
	m_prevPos = m_pos;
	m_acceleration = { 0.0f, 0.0f, 0.0f };

	m_mass = 1.0f;
	m_timeStep = _timeStep;	// FOR JC
	m_timeStepSquared = pow(_timeStep, 2.0f);
	m_damping = _damping;	
	m_dampingInverse = (1.0f - m_damping);

	// FOR JC
	m_ignited = false;
	m_lightTime = 0.0f;
	m_destroyTime = 0.0f;
	m_timeUntilFullyLit = 0.0f;
	m_timeUntilDestroyed = 0.0f;
	
	return true;
}

void Physics_Particle::Process()
{
	// Process movement only if the particle is not static
	if (m_static == false)
	{
		// Calculate and update position
		v3float temp = m_pos;
		m_pos = m_pos + ((m_pos - m_prevPos) * m_dampingInverse) + (m_acceleration * m_timeStepSquared);
		m_prevPos = temp;
		m_acceleration = { 0.0f, 0.0f, 0.0f};
	}

	if (m_ignited == true)
	{
		m_timeUntilFullyLit -= m_timeStep;
		m_timeUntilDestroyed -= m_timeStep;

		if (m_timeUntilFullyLit > 0.0f)
		{
			float ratio = 1 - (m_timeUntilFullyLit / m_lightTime);
			m_pVertex->color = d3dxColors::Red * ratio;
			m_pVertex->color.a = 1.0f;
		}
		else
		{
			float ratio =  (m_timeUntilDestroyed / m_destroyTime);
			m_pVertex->color.a = ratio;
		}
	}
}

void Physics_Particle::Move(v3float _movement)
{
	if (m_static == false)
	{
		// Apply movement only if the particle is not static
		m_pos += _movement;
	}
}

void Physics_Particle::SetSelectedPosition(v3float _pos)
{
	if (m_selected)
	{
		m_pos = _pos;
	}
}

void Physics_Particle::Ignite(float _burnTime)
{
	m_ignited = true;
	m_timeUntilFullyLit = _burnTime;
	m_lightTime = m_timeUntilFullyLit;
	m_timeUntilDestroyed = _burnTime * 4.0f;
	m_destroyTime = m_timeUntilDestroyed;
}

void Physics_Particle::Reset()
{
	m_ignited = false;
}
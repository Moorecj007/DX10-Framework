/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : Physics_Particle.cpp
* Description : Physics particle that is part of a cloth weave
* Author :	Callan Moore
* Mail :	Callan.Moore@mediadesign.school.nz
*/

// This Include
#include "Physics_Particle.h"

Physics_Particle::Physics_Particle()
{
	// Initialise all Pointers to NULL
	m_pVertex = 0;
}

Physics_Particle::~Physics_Particle()
{
}

bool Physics_Particle::Initialise(UINT _particleID, TVertexColor* _pVertex, v3float _pos, float _timeStep, float _damping, bool _static)
{
	if (_pVertex == 0)
	{
		// If the vertex is null then the initialization is invalid
		return false;
	}

	// Assign member variables
	m_particleID = _particleID;
	m_static = _static;
	m_pVertex = _pVertex;
	m_selected = false;

	m_pos = _pos;
	m_prevPos = m_pos;
	m_acceleration = { 0.0f, 0.0f, 0.0f };

	m_mass = 1.0f;
	m_timeStep = _timeStep;
	m_timeStepSquared = pow(_timeStep, 2.0f);
	m_damping = _damping;	
	m_dampingInverse = (1.0f - m_damping);

	// Set Ignition Variables
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
		// Calculate and update position using verlet integration
		v3float temp = m_pos;
		m_pos = m_pos + ((m_pos - m_prevPos) * m_dampingInverse) + (m_acceleration * m_timeStepSquared);
		m_prevPos = temp;
		m_acceleration = { 0.0f, 0.0f, 0.0f};
	}

	if (m_ignited == true)
	{
		// Reduce the burning timers
		m_timeUntilFullyLit -= m_timeStep;
		m_timeUntilDestroyed -= m_timeStep;

		if (m_timeUntilFullyLit > 0.0f)
		{
			// Calculate the inverse ratio of time left until completely lit
			float ratio =  m_timeUntilFullyLit / m_lightTime;

			// Edit the vertex color to be red based on the ratio (Redder the longer its been lit)
			m_pVertex->color.r = 1.0f;
			m_pVertex->color.g = ratio;
			m_pVertex->color.b = ratio;
			m_pVertex->color.a = 1.0f;	// Set the alpha back to full after ratio calculation
		}
		else
		{
			// Calculate the ratio of time left until destroyed
			float ratio =  m_timeUntilDestroyed / m_destroyTime;

			// Alpha the color based on the ratio so that the more transparent it vertex is the closer to being destroyed
			m_pVertex->color.r = ratio + 0.2f;
			m_pVertex->color.a = ratio + 0.1f;
		}
	}
}

void Physics_Particle::Move(v3float _movement)
{
	if (m_static == false && m_selected == false)
	{
		// Apply movement only if the particle is not static
		m_pos += _movement;
	}
}

void Physics_Particle::SetPosition(v3float _pos, bool _stopMovement)
{
	if (m_static == false)
	{
		m_pos = _pos;

		// Stops the movement by setting the old position to the newly set position as well so verlet integration doesn't calculate acceleration
		if (_stopMovement == true)
		{
			m_prevPos = m_pos;
		}
	}
}

void Physics_Particle::Ignite(float _burnTime)
{
	// Set the particle as ignited
	m_ignited = true;

	// Store the time that it takes become fully lit ( change to full red color)
	m_lightTime = _burnTime;
	m_timeUntilFullyLit = m_lightTime;
	
	// Store the time that it takes to be destroyed completely after being ignited
	m_destroyTime = _burnTime * 4.0f;
	m_timeUntilDestroyed = m_destroyTime;
}

void Physics_Particle::Reset()
{
	// Default states
	m_static = false;
	m_selected = false;
	m_ignited = false;
	m_acceleration *= 0;
}
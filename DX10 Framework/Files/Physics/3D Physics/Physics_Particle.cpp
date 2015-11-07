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
}

Physics_Particle::~Physics_Particle()
{
}

bool Physics_Particle::Initialise(int _particleID, TVertexColor* _pVertex, v3float _pos, float _timeStep, float _damping, bool _static)
{
	if (_pVertex == 0)
	{
		return false;
	}

	// Assign member variables
	m_particleID = _particleID;
	m_static = _static;
	m_pVertex = _pVertex;

	m_pos = _pos;
	m_prevPos = m_pos;
	m_acceleration = { 0.0f, 0.0f, 0.0f };

	m_mass = 1.0f;
	m_timeStepSquared = pow(_timeStep, 2.0f);
	m_damping = _damping;	
	m_dampingInverse = (1.0f - m_damping);

	m_ignited = false;

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